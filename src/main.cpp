#include <graphicssubsystem.h>
#include <utils.h>
#include <scenemanager.h>
#include <inputsubsystem.h>
#include <eventmanager.h>
#include <networksubsystem.h>

#include <game.h>

int main(int argc, char* argv[]) {

    GraphicsSubsystem::init(argc,argv);
    GraphicsSubsystem::createWindow(30,30,640,480, "GraphicsProject2013");
    GraphicsSubsystem::zBufferEnabled(true);

    GraphicsSubsystem::shadowMappingEnabled = false;

    NetworkSubsystem::init();

    EventManager::RegisterCommand("moveForward", moveForward);
    EventManager::RegisterCommand("moveBackward", moveBackward);
    EventManager::RegisterCommand("moveLeft", moveLeft);
    EventManager::RegisterCommand("moveRight", moveRight);
    InputSubsystem::init();

    NetworkSubsystem::connect("localhost");

    Game::init();

    GraphicsSubsystem::run();

    return 0;
}


