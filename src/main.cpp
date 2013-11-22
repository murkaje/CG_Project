#include <graphicssubsystem.h>
#include <utils.h>
#include <scenemanager.h>
#include <inputsubsystem.h>
#include <eventmanager.h>
#include <networksubsystem.h>

#include <game.h>

void moveLeft() {
    Game::get().localPlayer.moveVec.z() += Game::get().localPlayer.movePerSecond * GraphicsSubsystem::delta;
}

void moveRight() {
    Game::get().localPlayer.moveVec.z() += -Game::get().localPlayer.movePerSecond * GraphicsSubsystem::delta;
}

void moveBackward() {
    Game::get().localPlayer.moveVec.x() += Game::get().localPlayer.movePerSecond * GraphicsSubsystem::delta;
}

void moveForward() {
    Game::get().localPlayer.moveVec.x() += -Game::get().localPlayer.movePerSecond * GraphicsSubsystem::delta;
}

int main(int argc, char* argv[]) {

    Game::init();

    NetworkSubsystem::init();

    GraphicsSubsystem::init(argc,argv);
    GraphicsSubsystem::createWindow(30,30,640,480, "GraphicsProject2013");
    GraphicsSubsystem::zBufferEnabled(true);

    EventManager::RegisterCommand("moveForward", moveForward);
    EventManager::RegisterCommand("moveBackward", moveBackward);
    EventManager::RegisterCommand("moveLeft", moveLeft);
    EventManager::RegisterCommand("moveRight", moveRight);

    InputSubsystem::init();

    NetworkSubsystem::connect("localhost");

    SceneManager::createTestScene();
    SceneManager::CurrentScene().init();

    GraphicsSubsystem::run();

    NetworkSubsystem::shutdown();

    printf("Successful termination");
    return 0;
}


