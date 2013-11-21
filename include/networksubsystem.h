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

RakNet::BitStream& operator<<(RakNet::BitStream& out, Object& in);
RakNet::BitStream& operator>>(RakNet::BitStream& in, Object& out);

class NetworkSubsystem
{
protected:
private:
    static RakNet::RakPeerInterface *peer;

    static void synchronizeObjs(std::list<Object*> &objects);

    static void synchronizeCurrentScene();
public:
//temporarily public
    static RakNet::RPC3 *rpc;
    static RakNet::NetworkIDManager networkIDManager;

    static void parseIncomingPackets();
//stahp
    enum Messages
    {
        SYNC_MSG=ID_USER_PACKET_ENUM+1,
        INIT_SCENE_MSG=ID_USER_PACKET_ENUM+2
    };

    static RakNet::SystemAddress serverAddress;

    static bool isServer;

    static void init();

    static void startServer();

    static bool connect(const char* host);

    static void shutdown();

    friend class GraphicsSubsystem;
};

#endif
