#include "networksubsystem.h"
#include <scenemanager.h>
#include <synchronizer.h>

#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"

RakNet::RakPeerInterface* NetworkSubsystem::peer = NULL;
RakNet::RPC3* NetworkSubsystem::rpc = NULL;
RakNet::NetworkIDManager NetworkSubsystem::networkIDManager;
RakNet::SystemAddress NetworkSubsystem::serverAddress;

OnIncomingConnection NetworkSubsystem::onIncomingConnection = NULL;
OnConnectionLost NetworkSubsystem::onConnectionLost = NULL;

bool NetworkSubsystem::isServer = false;

RakNet::BitStream& operator<<(RakNet::BitStream& out, Object& in) {
    RakNet::RakString rs(in.name.c_str());
    out.Write(rs);
    RakNet::RakString rs2(in.tag.c_str());
    out.Write(rs2);
    unsigned int networkId = in.GetNetworkID();
    out.Write(networkId);
    unsigned short numComponents = in.components.size();
    out.Write(numComponents);
    for (std::map<std::string,Component*>::iterator comp = in.components.begin(); comp != in.components.end(); comp++) {
        RakNet::RakString compName(comp->first.c_str());
        out.Write(compName);
        int compTypeId = comp->second->typeId();
        out.Write(compTypeId);
        comp->second->writeTo(out);
    }
    unsigned short numChildren = in.getChildren().size();
    out.Write(numChildren);
    for (std::list<Object*>::iterator child = in.getChildren().begin(); child != in.getChildren().end(); child++) {
        out << *(*child);
    }
    return out;
}

RakNet::BitStream& operator>>(RakNet::BitStream& in, Object& out) {
    RakNet::RakString rs;
    in.Read(rs);
    out.name = rs.C_String();
    RakNet::RakString rs2;
    in.Read(rs2);
    out.tag = rs2.C_String();
    unsigned int networkId;
    in.Read(networkId);
    out.SetNetworkIDManager(&NetworkSubsystem::networkIDManager);
	out.SetNetworkID(networkId);
    unsigned short numComponents;
    in.Read(numComponents);
    for (int i = 0; i < numComponents; i++) {
        RakNet::RakString compName;
        in.Read(compName);
        int compTypeId = 0;
        in.Read(compTypeId);
        Component *comp = Component::allocate(compName.C_String(), compTypeId);
        comp->readFrom(in);
        out.addComponent(comp);
    }
    unsigned short numChildren;
    in.Read(numChildren);
    for (int i = 0; i < numChildren; i++) {
        Object *child = new Object();
        in >> *child;
        out.addChild(*child);
    }
    return in;
}

void NetworkSubsystem::init() {
    peer = RakNet::RakPeerInterface::GetInstance();
    rpc = new RakNet::RPC3;
    rpc->SetNetworkIDManager(&networkIDManager);

    RPC3_REGISTER_FUNCTION(NetworkSubsystem::rpc, RemoteInstantiate);
    RPC3_REGISTER_FUNCTION(NetworkSubsystem::rpc, RemoteDestroy);
    RPC3_REGISTER_FUNCTION(NetworkSubsystem::rpc, &Object::Synchronize);
}

void NetworkSubsystem::startServer() {
    printf("Starting the server on port %d.\n", SERVER_PORT);
    RakNet::SocketDescriptor sd(SERVER_PORT,0);
    //networkIDManager.SetIsNetworkIDAuthority(true);
    peer->Startup(MAX_CLIENTS, &sd, 1);
    peer->SetMaximumIncomingConnections(MAX_CLIENTS);
    isServer = true;
    peer->AttachPlugin(rpc);
}

bool NetworkSubsystem::connect(const char* host) {
    RakNet::SocketDescriptor sd;
    peer->Startup(1,&sd, 1);
    printf("Connecting...\n");
    double s = Utils::time();
    serverAddress = RakNet::SystemAddress(host,SERVER_PORT);
    peer->Connect(host, SERVER_PORT, 0,0);
    while (peer->GetConnectionState(serverAddress) != RakNet::IS_CONNECTED) {
        if (Utils::time()-s > 5000) {
            printf("Failed to connect to server on port %d.\n", SERVER_PORT);
            return false;
        }
    }
    printf("Connected to server on port %d.\n", SERVER_PORT);
    peer->AttachPlugin(rpc);
    return true;
}

void NetworkSubsystem::shutdown() {
    if (peer != NULL) peer->Shutdown(100, 0);
    RakNet::RakPeerInterface::DestroyInstance(peer);
    if (rpc != NULL) delete rpc;

}

void NetworkSubsystem::synchronizeObjs(std::list<Object*> &objects) {
    for (std::list<Object*>::iterator obj = objects.begin(); obj != objects.end(); obj++) {
        Synchronizer *s = Synchronizer::get(*(*obj));
        if (s != NULL) {
            s->bs.Reset();
            (*obj)->Synchronize(&s->bs);
        }
        synchronizeObjs((*obj)->getChildren());
    }
}

void NetworkSubsystem::synchronizeCurrentScene() {
    unsigned short numConnections = 0;
    peer->GetConnectionList(NULL, &numConnections);
    if (isServer && numConnections > 0) {
        rpc->SetRecipientAddress(peer->GetMyBoundAddress(),true);
        synchronizeObjs(SceneManager::CurrentScene().getObjsList());
    } else if (peer->GetConnectionState(serverAddress) == RakNet::IS_CONNECTED) {
        rpc->SetRecipientAddress(serverAddress,false);
        synchronizeObjs(SceneManager::CurrentScene().getObjsList());
    }
}

void NetworkSubsystem::parseIncomingPackets() {
    RakNet::Packet *packet;
    for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive()) {
        switch (packet->data[0]) {
        case ID_REMOTE_DISCONNECTION_NOTIFICATION:
            printf("Another client has disconnected.\n");
            break;
        case ID_REMOTE_CONNECTION_LOST:
            printf("Another client has lost the connection.\n");
            break;
        case ID_REMOTE_NEW_INCOMING_CONNECTION:
            printf("Another client has connected.\n");
            break;
        case ID_CONNECTION_REQUEST_ACCEPTED:
            printf("Our connection request has been accepted.\n");
            break;
        case ID_NEW_INCOMING_CONNECTION:
            {
                printf("A connection is incoming.\n");
                if (onIncomingConnection != NULL) {
                    onIncomingConnection(packet->systemAddress.ToString());
                }
            }
            break;
        case ID_NO_FREE_INCOMING_CONNECTIONS:
            printf("The server is full.\n");
            break;
        case ID_DISCONNECTION_NOTIFICATION:
            if (isServer) printf("A client has disconnected.\n");
            else printf("We have been disconnected.\n");
            break;
        case ID_CONNECTION_LOST:
            if (isServer) {
                printf("A client lost the connection.\n");
                if (onConnectionLost != NULL) {
                    onConnectionLost(packet->systemAddress.ToString());
                }
            } else printf("Connection lost.\n");
            break;
        case ID_RPC_REMOTE_ERROR:
         {
            // Recipient system returned an error
            switch (packet->data[1])
            {
            case RakNet::RPC_ERROR_NETWORK_ID_MANAGER_UNAVAILABLE:
               printf("RPC_ERROR_NETWORK_ID_MANAGER_UNAVAILABLE\n");
               break;
            case RakNet::RPC_ERROR_OBJECT_DOES_NOT_EXIST:
               printf("RPC_ERROR_OBJECT_DOES_NOT_EXIST\n");
               break;
            case RakNet::RPC_ERROR_FUNCTION_INDEX_OUT_OF_RANGE:
               printf("RPC_ERROR_FUNCTION_INDEX_OUT_OF_RANGE\n");
               break;
            case RakNet::RPC_ERROR_FUNCTION_NOT_REGISTERED:
               printf("RPC_ERROR_FUNCTION_NOT_REGISTERED\n");
               break;
            case RakNet::RPC_ERROR_FUNCTION_NO_LONGER_REGISTERED:
               printf("RPC_ERROR_FUNCTION_NO_LONGER_REGISTERED\n");
               break;
            case RakNet::RPC_ERROR_CALLING_CPP_AS_C:
               printf("RPC_ERROR_CALLING_CPP_AS_C\n");
               break;
            case RakNet::RPC_ERROR_CALLING_C_AS_CPP:
               printf("RPC_ERROR_CALLING_C_AS_CPP\n");
               break;
            }
            printf("Function: %s", packet->data+2);
         }
        default:
            printf("Message with identifier %i has arrived.\n", packet->data[0]);
            break;
        }
    }
}
