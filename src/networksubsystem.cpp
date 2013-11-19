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

bool NetworkSubsystem::isServer = false;
bool NetworkSubsystem::isClient = false;

RakNet::BitStream& operator<<(RakNet::BitStream& out, Object& in) {
    RakNet::RakString rs(in.name.c_str());
    out.Write(rs);
    unsigned int networkId = in.networkId;
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
    printf("Connecting...");
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
    isClient = true;
    peer->AttachPlugin(rpc);
    return true;
}

void NetworkSubsystem::shutdown() {
    if (peer != NULL) peer->Shutdown(100, 0);
    RakNet::RakPeerInterface::DestroyInstance(peer);
    if (rpc != NULL) delete rpc;

}

void NetworkSubsystem::synchronizeObjs(std::list<Object*> &objects, RakNet::BitStream &bs, bool write) {
    for (std::list<Object*>::iterator obj = objects.begin(); obj != objects.end(); obj++) {
        Synchronizer *s = Synchronizer::get(*(*obj));
        if (s != NULL) {
            if (write)
                s->write(bs);
            else
                s->read(bs);
        }
        synchronizeObjs((*obj)->getChildren(), bs, write);
    }
}

void NetworkSubsystem::synchronizeCurrentScene() {
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)SYNC_MSG);
    synchronizeObjs(SceneManager::CurrentScene().getObjsList(), bsOut, true);
    peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,peer->GetMyGUID(),false);
}

void initializeSceneOnClient(RakNet::SystemAddress clientAddress) {
    NetworkSubsystem::rpc->SetRecipientAddress(clientAddress, false);
    std::list<Object*>::iterator obj = SceneManager::CurrentScene().getObjsList().begin();
    for (; obj != SceneManager::CurrentScene().getObjsList().end(); obj++) {
        printf("calling instantiate for %s on %s\n", (*obj)->name.c_str(), clientAddress.ToString());
        NetworkSubsystem::rpc->CallC("Instantiate", *(*obj));
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
                initializeSceneOnClient(packet->systemAddress);
            }
            break;
        case ID_NO_FREE_INCOMING_CONNECTIONS:
            printf("The server is full.\n");
            break;
        case ID_DISCONNECTION_NOTIFICATION:
            if (isServer) printf("A client has disconnected.\n");
            if (isClient) printf("We have been disconnected.\n");
            break;
        case ID_CONNECTION_LOST:
            if (isServer) printf("A client lost the connection.\n");
            if (isClient) printf("Connection lost.\n");
            break;
        case SYNC_MSG:
            {
                RakNet::BitStream bsIn(packet->data,packet->length,false);
                bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
                synchronizeObjs(SceneManager::CurrentScene().getObjsList(), bsIn, false);
            }
            break;
        case INIT_SCENE_MSG:
            {
                RakNet::BitStream bsIn(packet->data,packet->length,false);
                bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

            }
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
