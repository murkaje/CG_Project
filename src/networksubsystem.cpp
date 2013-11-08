#include "networksubsystem.h"

#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"

RakNet::RakPeerInterface* NetworkSubsystem::peer = NULL;

void NetworkSubsystem::init() {
    peer = RakNet::RakPeerInterface::GetInstance();

}

void NetworkSubsystem::startServer() {
    printf("Starting the server on port %d.\n", SERVER_PORT);
    RakNet::SocketDescriptor sd(SERVER_PORT,0);
    peer->Startup(MAX_CLIENTS, &sd, 1);
    peer->SetMaximumIncomingConnections(MAX_CLIENTS);

}

void NetworkSubsystem::connect(const char* host) {
    //RakNet::SocketDescriptor sd;
    //peer->Startup(1,&sd, 1);
    printf("Connecting to server on port %d.\n", SERVER_PORT);
    peer->Connect(host, SERVER_PORT, 0,0);

}

void NetworkSubsystem::shutdown() {
    RakNet::RakPeerInterface::DestroyInstance(peer);

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
            {
                RakNet::BitStream bsOut;
                bsOut.Write((RakNet::MessageID)MSG_1);
                bsOut.Write("Hello world");
                peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
            }
            break;
        case ID_NEW_INCOMING_CONNECTION:
            printf("A connection is incoming.\n");
            break;
        case ID_NO_FREE_INCOMING_CONNECTIONS:
            printf("The server is full.\n");
            break;
        case ID_DISCONNECTION_NOTIFICATION:
            //if (isServer){
                printf("A client has disconnected.\n");
            //} else {
            //    printf("We have been disconnected.\n");
            //}
            break;
        case ID_CONNECTION_LOST:
            //if (isServer){
                printf("A client lost the connection.\n");
            //} else {
            //    printf("Connection lost.\n");
            //}
            break;
        case MSG_1:
            {
                RakNet::RakString rs;
                RakNet::BitStream bsIn(packet->data,packet->length,false);
                bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
                bsIn.Read(rs);
                printf("%s\n", rs.C_String());
            }
            break;
        default:
            printf("Message with identifier %i has arrived.\n", packet->data[0]);
            break;
        }
    }
}
