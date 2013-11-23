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

Game::Game(): localPlayer("localPlayer")
{
    //ctor
}

Game::~Game()
{
    //dtor
}

void Game::init() {
    instance = new Game();
}

Game& Game::get() {
    return *instance;
}

Game::Player::Player(std::string name): moveVec(0), name(name) {

}

Object* Game::Player::createCharacter(std::string name, std::string addr) {

    Object *sphereObj = GeometricShape::createSphere(vec3f(0,0.5,0), vec3f(0,45,0), vec3f(1,1,1),vec3f(0,1,0));
    sphereObj->name = name;
    sphereObj->tag = addr;
    Behavior::addLocal(sphereObj, "moveObject", moveObject);
    Collider::addBox(*sphereObj);
    Behavior::add(sphereObj, "colorCollidingObjects");
    //Synchronizer::add(sphereObj, objSaysHello);
    Synchronizer::add(sphereObj, "movementSynchronizer");

    Object *light = Light::createPointLight(vec3f(0,1,0));
    Object *lightBall = GeometricShape::createSphere(vec3f(0), vec3f(0), vec3f(0.1,0.1,0.1),vec3f(1,1,0));
    Renderer::get(*lightBall)->material.lighting_enabled = false;
    light->addChild(*lightBall);
    sphereObj->addChild(*light);

    return sphereObj;
}


void Game::attachCamera(Object &obj) {
    //Object *camera = Camera::createOrthographicCamera(10, 0.5, 100);
    Object *camera = Camera::createPerspectiveCamera(45, 4.0/3.0, 0.5, 100);
    Transform::setObjPosition(camera, vec3f(0, 6, 6));
    Transform::setObjRotation(camera, vec3f(-45, 0, 0));
    camera->name = "MainCamera";
    obj.addChild(*camera);
}
