#ifndef NETWORKSUBSYSTEM_H
#define NETWORKSUBSYSTEM_H

#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RPC3.h"
#include "NetworkIDManager.h"

#include <graphicssubsystem.h>
#include <object.h>

#include <list>

#define MAX_CLIENTS 10
#define SERVER_PORT 60000

class NetworkSubsystem
{
protected:
private:
    static RakNet::RakPeerInterface *peer;

    static void synchronizeObjs(std::list<Object> &objects, RakNet::BitStream &bs, bool write);

    static void synchronizeCurrentScene();
public:
//temporarily public
    static RakNet::RPC3 *rpc;
    static RakNet::NetworkIDManager networkIDManager;

    static void parseIncomingPackets();
//stahp
    enum Messages
    {
        MSG_1=ID_USER_PACKET_ENUM+1
    };

    static bool isServer, isClient;

    static void init();

    static void startServer();

    static bool connect(const char* host);

    static void shutdown();

    friend class GraphicsSubsystem;
};

#endif
