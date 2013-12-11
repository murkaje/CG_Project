#include <graphicssubsystem.h>
#include <utils.h>
#include <scenemanager.h>
#include <inputsubsystem.h>
#include <eventmanager.h>
#include <networksubsystem.h>

#include <game.h>

int main(int argc, char* argv[]) {

    GraphicsSubsystem::init(argc,argv);
    GraphicsSubsystem::createWindow(0,0,800,600, "GraphicsProject2013");
    GraphicsSubsystem::zBufferEnabled(true);

    GraphicsSubsystem::shadowMappingEnabled = false;

    NetworkSubsystem::init();

    EventManager::RegisterCommand("moveForward", moveForward);
    EventManager::RegisterCommand("moveBackward", moveBackward);
    EventManager::RegisterCommand("moveLeft", moveLeft);
    EventManager::RegisterCommand("moveRight", moveRight);

    EventManager::RegisterCommand("rotateLeft", rotateLeft);
    EventManager::RegisterCommand("rotateRight", rotateRight);

    InputSubsystem::init();

    NetworkSubsystem::connect("localhost");

    Game::init();

    GraphicsSubsystem::run();

    return 0;
}


