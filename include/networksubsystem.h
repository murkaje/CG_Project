#ifndef NETWORKSUBSYSTEM_H
#define NETWORKSUBSYSTEM_H

#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"

#include <graphicssubsystem.h>

#define MAX_CLIENTS 10
#define SERVER_PORT 60000

class NetworkSubsystem
{
protected:
private:
    static RakNet::RakPeerInterface *peer;

    static void parseIncomingPackets();

public:
    enum Messages
    {
        MSG_1=ID_USER_PACKET_ENUM+1
    };

    static void init();

    static void startServer();

    static void connect(const char* host);

    static void shutdown();

    friend class GraphicsSubsystem;
};

#endif
