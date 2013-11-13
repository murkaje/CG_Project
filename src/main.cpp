
#include <utils.h>
#include <iostream>
#include <scene.h>
#include <vector3f.h>
#include <transform.h>
#include <mesh.h>
#include <renderer.h>
#include <behavior.h>
#include <graphicssubsystem.h>
#include <scenemanager.h>
#include <inputsubsystem.h>
#include <camera.h>
#include <light.h>
#include <collider.h>
#include <eventmanager.h>
#include <math.h>
#include <networksubsystem.h>
#include <synchronizer.h>

#include "BitStream.h"

v3f moveVec = v3f::zero;
int movePerSecond = 2;

void moveLeft()
{
    moveVec.z += movePerSecond * GraphicsSubsystem::delta;
}
void moveRight()
{
    moveVec.z += -movePerSecond * GraphicsSubsystem::delta;
}
void moveBackward()
{
    moveVec.x += movePerSecond * GraphicsSubsystem::delta;
}
void moveForward()
{
    moveVec.x += -movePerSecond * GraphicsSubsystem::delta;
}

void rotateObject(Object &obj)
{
    int degreesPerSecond = 90;
    Transform::rotateObj(&obj, 0, 0, degreesPerSecond*GraphicsSubsystem::delta);
}

void moveObject(Object &obj)
{
    Collider *c = Collider::get(obj);
    std::list<Collider::Collision>::iterator col = c->collisions().begin();
    v3f newPos = Transform::get(obj)->position+moveVec;
    for (; col != c->collisions().end(); col++) {
        //alter movement vector so we cannot go beyond collision points
        //printf("collision normal %f %f %f \n", col->normal.x, col->normal.y, col->normal.z);
        Transform *otherCol = Transform::get(*col->with.owner());
        if (otherCol->position.x > newPos.x && newPos.x >= col->point.x) moveVec.x += moveVec.x*col->normal.x; //x+
        else if (otherCol->position.x < newPos.x && newPos.x <= col->point.x) moveVec.x -= moveVec.x*col->normal.x; //x-
        if (otherCol->position.z > newPos.z && newPos.z >= col->point.z) moveVec.z += moveVec.z*col->normal.z; //z+
        else if (otherCol->position.z < newPos.z && newPos.z <= col->point.z) moveVec.z -= moveVec.z*col->normal.z; //z-
    }
    Transform::translateObj(&obj, moveVec.x, moveVec.y, moveVec.z);
    //reset movement vector for next frame
    moveVec = v3f::zero;
}

void colorCollidingObjects(Object &obj) {
    Collider *c = Collider::get(obj);
    std::list<Collider::Collision>::iterator col = c->collisions().begin();
    for (; col != c->collisions().end(); col++) {
        //printf("colliding with %s\n", col->with.owner()->name.c_str());
        Renderer::get(*col->with.owner())->material.diffuse = v3f(0,1,0);
    }
}

void resetColorIfNoCollisions(Object &obj) {
    Collider *c = Collider::get(obj);
    if (c->collisions().size() == 0) Renderer::get(obj)->material.diffuse = v3f(1,0,0);
}

void objSaysHello(Object& obj, RakNet::BitStream &bs, bool write) {
    if (write) {
        bs.Write((std::string("hello from ")+obj.name).c_str());
    } else {
        RakNet::RakString rs;
        bs.Read(rs);
        printf("%s got a %s\n", obj.name.c_str(), rs.C_String());
    }
}

void synchronizeTransform(Object& obj, RakNet::BitStream &bs, bool write) {
    Transform *t = Transform::get(obj);
    if (t != NULL) {
        if (write) {
            bs.WriteVector(t->position.x,t->position.y,t->position.z);
            bs.WriteVector(t->rotation.x,t->rotation.y,t->rotation.z);
            bs.WriteVector(t->scale.x,t->scale.y,t->scale.z);
        } else {
            bs.ReadVector(t->position.x,t->position.y,t->position.z);
            bs.ReadVector(t->rotation.x,t->rotation.y,t->rotation.z);
            bs.ReadVector(t->scale.x,t->scale.y,t->scale.z);
        }
    }

}

int main(int argc, char* argv[])
{

    GraphicsSubsystem::init(argc,argv);
    GraphicsSubsystem::createWindow(30,30,640,480, "GraphicsProject2013");
    GraphicsSubsystem::zBufferEnabled(true);

    EventManager::RegisterCommand("moveForward", moveForward);
    EventManager::RegisterCommand("moveBackward", moveBackward);
    EventManager::RegisterCommand("moveLeft", moveLeft);
    EventManager::RegisterCommand("moveRight", moveRight);

    InputSubsystem::init();

    NetworkSubsystem::init();
    NetworkSubsystem::startServer();

    NetworkSubsystem::connect("localhost");

    //The plane "quad mesh" doesn't really work with lighting properly, super thin cube for now instead :(
    //Object *planeObj = GeometricShape::createPlane(v3f::zero,v3f::zero,v3f(6,0,6),v3f(0.5,0.5,0.5));
    Object *planeObj = GeometricShape::createCube(v3f::zero,v3f::zero, v3f(25,0.001,25),v3f(0.5,0.5,0.5));
    planeObj->name = "plane";
    //make the plane really shiny
    Renderer::get(*planeObj)->material.specular = v3f(1,1,1);
    Renderer::get(*planeObj)->material.shininess = 100;

    Object *sphereObj = GeometricShape::createSphere(v3f(0,0.5,0), v3f(0,45,0), v3f(1,1,1),v3f(0,1,0));
    Behavior::add(sphereObj, moveObject);
    Collider::addBox(*sphereObj);
    Behavior::add(sphereObj, colorCollidingObjects);
    //Synchronizer::add(sphereObj, objSaysHello);
    Synchronizer::add(sphereObj, synchronizeTransform);

    Object *cubeObj = GeometricShape::createCube(v3f(2,1,2), v3f::zero, v3f(1,1,1),v3f(1,0,0));
    Behavior::add(cubeObj, rotateObject);
    Collider::addBox(*cubeObj);
    Behavior::add(cubeObj, resetColorIfNoCollisions);
    //Synchronizer::add(cubeObj, objSaysHello);

    Object *secondCubeObj = GeometricShape::createCube(v3f(2,0.5,0), v3f::zero, v3f(3.5,.5,.5),v3f(1,0,1));
    Collider::addBox(*secondCubeObj);
    Behavior::add(secondCubeObj, resetColorIfNoCollisions);

    //Object *camera = Camera::createOrthographicCamera(10, 0.5, 100);
    Object *camera = Camera::createPerspectiveCamera(45, 4.0/3.0, 0.5, 100);
    Transform::setObjPosition(camera, 0, 6, 6);
    Transform::setObjRotation(camera, -45, 0, 0);
    camera->name = "MainCamera";
    //SceneManager::testScene.addObject(camera);
    sphereObj->addChild(camera);

    Object *light = Light::createPointLight(v3f(0,1,0));
    Object *lightBall = GeometricShape::createSphere(v3f(0,0,0), v3f::zero, v3f(0.1,0.1,0.1),v3f(1,1,0));
    Renderer::get(*lightBall)->material.lighting_enabled = false;
    light->addChild(lightBall);
    sphereObj->addChild(light);
    //SceneManager::testScene.addObject(light);
    SceneManager::testScene.addObject(planeObj);
    SceneManager::testScene.addObject(sphereObj);
    SceneManager::testScene.addObject(cubeObj);
    SceneManager::testScene.addObject(secondCubeObj);

    SceneManager::testScene.init();
    GraphicsSubsystem::run();

    NetworkSubsystem::shutdown();

    printf("Successful termination");
    return 0;
}


