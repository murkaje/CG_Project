
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

v3f spherePos = v3f(0,0.5,0);
int movePerSecond = 2;

void moveLeft()
{
    spherePos.z += movePerSecond * GraphicsSubsystem::delta;
}
void moveRight()
{
    spherePos.z += -movePerSecond * GraphicsSubsystem::delta;
}
void moveBackward()
{
    spherePos.x += movePerSecond * GraphicsSubsystem::delta;
}
void moveForward()
{
    spherePos.x += -movePerSecond * GraphicsSubsystem::delta;
}

void rotateObject(Object &obj)
{
    int degreesPerSecond = 90;
    Transform::rotateObj(&obj, 0,0, degreesPerSecond*GraphicsSubsystem::delta);
}

void moveObject(Object &obj)
{
    Transform::setObjPosition(&obj, spherePos.x, spherePos.y, spherePos.z);
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

    //The plane "quad mesh" doesn't really work with lighting properly, super thin cube for now instead :(
    //Object *planeObj = GeometricShape::createPlane(v3f::zero,v3f::zero,v3f(6,0,6),v3f(0.5,0.5,0.5));
    Object *planeObj = GeometricShape::createCube(v3f::zero,v3f::zero, v3f(25,0.001,25),v3f(0.5,0.5,0.5));
    planeObj->name = "plane"; //rename the cube, so the current collision test wont trigger
    //make the plane really shiny
    ((Renderer*)planeObj->getComponent(Component::RENDERER))->material.specular = v3f(1,1,1);
    ((Renderer*)planeObj->getComponent(Component::RENDERER))->material.shininess = 100;

    Object *sphereObj = GeometricShape::createSphere(spherePos, v3f(0,45,0), v3f(1,1,1),v3f(0,1,0));
    Behavior::add(sphereObj, moveObject);

    Object *cubeObj = GeometricShape::createCube(v3f(2,1,2), v3f::zero, v3f(1,1,1),v3f(1,0,0));
    Behavior::add(cubeObj, rotateObject);

    Object *secondCubeObj = GeometricShape::createCube(v3f(2,0.5,0), v3f::zero, v3f(.5,.5,.5),v3f(1,0,1));

    //Object *camera = Camera::createOrthographicCamera(10, 0.5, 100);
    Object *camera = Camera::createPerspectiveCamera(45, 4.0/3.0, 0.5, 100);
    Transform::setObjPosition(camera, 0, 6, 6);
    Transform::setObjRotation(camera, -45, 0, 0);
    camera->name = "MainCamera";
    //SceneManager::testScene.addObject(camera);
    sphereObj->addChild(camera);

    Object *light = Light::createPointLight(v3f(0,1,0));
    light->addChild(GeometricShape::createSphere(v3f(0,0,0), v3f::zero, v3f(0.1,0.1,0.1),v3f(1,1,0)));
    sphereObj->addChild(light);
    //SceneManager::testScene.addObject(light);
    SceneManager::testScene.addObject(planeObj);
    SceneManager::testScene.addObject(sphereObj);
    SceneManager::testScene.addObject(cubeObj);
    SceneManager::testScene.addObject(secondCubeObj);

    SceneManager::testScene.init();
    GraphicsSubsystem::run();

    return 0;
}


