#include <utils.h>
#include <graphicssubsystem.h>
#include <scenemanager.h>
#include <networksubsystem.h>

#include <funcdefs.h>

int main(int argc, char* argv[])
{
    Game::init();

    NetworkSubsystem::init();
    NetworkSubsystem::onIncomingConnection = OnPlayerConnected;
    NetworkSubsystem::onConnectionLost = OnPlayerDisconnected;
    NetworkSubsystem::startServer();

    GraphicsSubsystem::init(argc,argv);
    GraphicsSubsystem::createWindow(30,30,30,30, "GraphicsProject2013Server");

    SceneManager::createTestScene();

    GraphicsSubsystem::run();

    NetworkSubsystem::shutdown();

    printf("Successful termination");
    return 0;
}


