#include <graphicssubsystem.h>
#include <utils.h>
#include <vec.h>
#include <behavior.h>
#include <scenemanager.h>
#include <inputsubsystem.h>
#include <eventmanager.h>
#include <networksubsystem.h>
#include <synchronizer.h>

#include "funcdefs.h"

int main(int argc, char* argv[]) {

    NetworkSubsystem::init();

    GraphicsSubsystem::init(argc,argv);
    GraphicsSubsystem::createWindow(30,30,640,480, "GraphicsProject2013");
    GraphicsSubsystem::zBufferEnabled(true);

    EventManager::RegisterCommand("moveForward", moveForward);
    EventManager::RegisterCommand("moveBackward", moveBackward);
    EventManager::RegisterCommand("moveLeft", moveLeft);
    EventManager::RegisterCommand("moveRight", moveRight);

    InputSubsystem::init();

    //Behavior::Register("moveObject", moveObject);
    Behavior::Register("colorCollidingObjects", colorCollidingObjects);
    //Synchronizer::Register("objSaysHello", objSaysHello);
    Synchronizer::Register("movementSynchronizer", movementSynchronizer);
    Behavior::Register("rotateObject", rotateObject);
    Behavior::Register("resetColorIfNoCollisions", resetColorIfNoCollisions);

    NetworkSubsystem::connect("localhost");

    SceneManager::CurrentScene().init();
    GraphicsSubsystem::run();

    NetworkSubsystem::shutdown();

    printf("Successful termination");
    return 0;
}


