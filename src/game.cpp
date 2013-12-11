#include "game.h"
#include "object.h"
#include "component.h"
#include "collider.h"
#include "renderer.h"
#include "behavior.h"
#include "transform.h"
#include "synchronizer.h"
#include "light.h"
#include "camera.h"
#include "vec.h"

#include "funcdefs.h"

Game* Game::instance = NULL;


void moveLeft() {
    Game::get().localPlayer.moveVec.z = std::min(Game::get().localPlayer.moveVec.z+1, 1.0f);
}

void moveRight() {
    Game::get().localPlayer.moveVec.z = std::max(Game::get().localPlayer.moveVec.z-1, -1.0f);
}

void moveBackward() {
    Game::get().localPlayer.moveVec.x = std::min(Game::get().localPlayer.moveVec.x+1, 1.0f);
}

void moveForward() {
    Game::get().localPlayer.moveVec.x = std::max(Game::get().localPlayer.moveVec.x-1, -1.0f);
}

void rotateLeft() {
    Transform *t = Transform::get(*Object::Find(Game::get().localPlayer.playerObj, "MainCamera")->parent());
    t->rotation.y += Game::get().localPlayer.rotationSpeed*GraphicsSubsystem::delta;
}

void rotateRight() {
    Transform *t = Transform::get(*Object::Find(Game::get().localPlayer.playerObj, "MainCamera")->parent());
    t->rotation.y -= Game::get().localPlayer.rotationSpeed*GraphicsSubsystem::delta;
}

Game::Game(): localPlayer("localPlayer") {

}

Game::~Game() {

}

void Game::init() {
    instance = new Game();

    Synchronizer::Register("movementSynchronizer", movementSynchronizer);
    //Behavior::Register("colorCollidingObjects", colorCollidingObjects);

    Object *planeObj = GeometricShape::createPlane(glm::vec3(0),glm::vec3(0), glm::vec3(25,1,25),glm::vec3(0.5));
    planeObj->name = "plane";
    //make the plane really shiny
    Renderer::get(*planeObj)->material.specular = glm::vec3(1);
    Renderer::get(*planeObj)->material.shininess = 100;
    Renderer::get(*planeObj)->cast_shadows = false;

    Renderer::get(*planeObj)->material.setTexture("../data/texturez_3851.jpg");

    Object *cubeObj = GeometricShape::createCube(glm::vec3(-2,1,2), glm::vec3(0),
                                                 glm::vec3(1,1,1), glm::vec3(1,0,0));
    Behavior::addLocal(cubeObj, "rotateObject", rotateObject);
    Collider::addBox(*cubeObj);
    //Behavior::addLocal(cubeObj, "resetColorIfNoCollisions", resetColorIfNoCollisions);
    Renderer::get(*cubeObj)->material.setTexture("../data/concrete.jpg");

    Object *secondCubeObj = GeometricShape::createCube(glm::vec3(2,1.0,0), glm::vec3(0),
                                                       glm::vec3(3.5,2.0,1.5), glm::vec3(1,0,1));
    Collider::addBox(*secondCubeObj);
   // Behavior::addLocal(secondCubeObj, "resetColorIfNoCollisions");
    Renderer::get(*secondCubeObj)->material.setTexture("../data/texturez_3851.jpg");

    Instantiate(*planeObj);
    Instantiate(*cubeObj);
    Instantiate(*secondCubeObj);
}

Game& Game::get() {
    return *instance;
}

Game::Player::Player(std::string name): moveVec(0), name(name) {

}

Object* Game::Player::createCharacter(std::string name, std::string addr) {

    Object *sphereObj = GeometricShape::createSphere(glm::vec3(2,0.5,-2), glm::vec3(0,45,0),
                                                     glm::vec3(1,1,1), glm::vec3(0,1,0));
    sphereObj->name = name;
    sphereObj->tag = addr;
    Behavior::addLocal(sphereObj, "moveObject", moveObject);
    Collider::addBox(*sphereObj);
    //Behavior::add(sphereObj, "colorCollidingObjects");
    Synchronizer::add(sphereObj, "movementSynchronizer");

    Object *light = Light::createPointLight(glm::vec3(0,1,0));
    Object *lightBall = GeometricShape::createSphere(glm::vec3(0), glm::vec3(0),
                                                     glm::vec3(0.1,0.1,0.1), glm::vec3(1,1,0));
    Renderer::get(*lightBall)->material.lighting_enabled = false;
    light->addChild(*lightBall);
    sphereObj->addChild(*light);
    Renderer::get(*lightBall)->cast_shadows = false;
    Renderer::get(*sphereObj)->receive_shadows = false;

    return sphereObj;
}

void Game::attachCamera(Object &obj) {
    //Object *camera = Camera::createOrthographicCamera(10, 0.5, 100);
    Object *camera = Camera::createPerspectiveCamera(45, 4.0/3.0, 0.5, 100);
    Transform::setObjPosition(camera, glm::vec3(0, 6, 6));
    Transform::setObjRotation(camera, glm::vec3(-45, 0, 0));
    camera->name = "MainCamera";
    obj.addChild(*camera);
}
