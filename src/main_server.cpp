#include <utils.h>
#include <graphicssubsystem.h>
#include <scenemanager.h>
#include <networksubsystem.h>

int main(int argc, char* argv[])
{
    NetworkSubsystem::init();
    NetworkSubsystem::startServer();

    GraphicsSubsystem::init(argc,argv);
    GraphicsSubsystem::createWindow(30,30,30,30, "GraphicsProject2013Server");

    //InputSubsystem::init();

    SceneManager::createTestScene();

    GraphicsSubsystem::run();

    NetworkSubsystem::shutdown();

    printf("Successful termination");
    return 0;
}


