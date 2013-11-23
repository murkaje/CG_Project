#include <graphicssubsystem.h>
#include <utils.h>
#include <scenemanager.h>
#include <inputsubsystem.h>
#include <eventmanager.h>
#include <networksubsystem.h>

#include <game.h>

void moveLeft() {
    Game::get().localPlayer.moveVec.z() = std::min(Game::get().localPlayer.moveVec.z()+1, 1.0f);
}

void moveRight() {
    Game::get().localPlayer.moveVec.z() = std::max(Game::get().localPlayer.moveVec.z()-1, -1.0f);
}

void moveBackward() {
    Game::get().localPlayer.moveVec.x() = std::min(Game::get().localPlayer.moveVec.x()+1, 1.0f);
}

void moveForward() {
    Game::get().localPlayer.moveVec.x() = std::max(Game::get().localPlayer.moveVec.x()-1, -1.0f);
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


