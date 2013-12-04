#include <utils.h>
#include <unistd.h>
#include <graphicssubsystem.h>
#include <scenemanager.h>
#include <physicssubsystem.h>
#include <networksubsystem.h>

#include <funcdefs.h>

#include <light.h>
#include <behavior.h>

const int SV_FPS = 30;

double frameStart, counter;
int frames, fps;

int main(int argc, char* argv[])
{
    GraphicsSubsystem::init(argc, argv);

    NetworkSubsystem::init();
    NetworkSubsystem::onIncomingConnection = OnPlayerConnected;
    NetworkSubsystem::onConnectionLost = OnPlayerDisconnected;
    NetworkSubsystem::onDisconnectionNotification = OnPlayerDisconnected;
    NetworkSubsystem::startServer();

    Game::init();

    std::cout<<"starting main loop\n";
    frameStart = Utils::time();
    while (true) {
        if (frames == 0) {
            counter = Utils::time();
        }

        if (counter+1000 >= Utils::time()) {
            frames++;
        } else {
            fps = frames;
            frames = 0;
        }
        NetworkSubsystem::parseIncomingPackets();

        PhysicsSubsystem::PerformPhysicsChecks();
        SceneManager::CurrentScene().update();

        NetworkSubsystem::synchronizeCurrentScene();

        double needed = frameStart+(1000.0/SV_FPS);
        if (needed >= Utils::time()) {
            double d = (needed-Utils::time())*1000;
            usleep(d);
        }
        GraphicsSubsystem::delta = (Utils::time()-frameStart)/1000;
        frameStart = Utils::time();
    }

    NetworkSubsystem::shutdown();

    return 0;
}


