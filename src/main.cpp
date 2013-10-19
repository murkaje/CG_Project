#include <GL/freeglut.h>
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

#define v3f vector3f

//Eventually a player should register move functions
void moveLeft() {
    std::cout<<"Moving left\n";
}
void moveRight() {
    std::cout<<"Moving right\n";
}
void moveBackward() {
    std::cout<<"Moving backward\n";
}
void moveForward() {
    std::cout<<"Moving forward\n";
}

void rotateObject(Object &obj)
{
    int degreesPerSecond = 90;
    Transform *t = (Transform*)obj.getComponent(Component::TRANSFORM);
    t->rotation.z += degreesPerSecond*GraphicsSubsystem::delta;
}

void moveObject(Object &obj)
{
    Transform *t = (Transform*)obj.getComponent(Component::TRANSFORM);
    int movePerSecond = 2;
    if(InputSubsystem::keyState[GLUT_KEY_UP+256] == true) {
        t->position.x += -movePerSecond * GraphicsSubsystem::delta;
    }
    if(InputSubsystem::keyState[GLUT_KEY_DOWN+256] == true) {
        t->position.x += movePerSecond * GraphicsSubsystem::delta;
    }
    if(InputSubsystem::keyState[GLUT_KEY_RIGHT+256] == true) {
        t->position.z += -movePerSecond * GraphicsSubsystem::delta;
    }
    if(InputSubsystem::keyState[GLUT_KEY_LEFT+256] == true) {
        t->position.z += movePerSecond * GraphicsSubsystem::delta;
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

    Object *planeObj = GeometricShape::createPlane(v3f::zero,v3f::zero,v3f(6,6,6),v3f(0.5,0.5,0.5));

    Object *sphereObj = GeometricShape::createSphere(v3f(0,1,0), v3f::zero, v3f(1,1,1),v3f(0,1,0));
    sphereObj->addComponent(new Behavior(moveObject));

    Object *cubeObj = GeometricShape::createCube(v3f(2,1,2), v3f::zero, v3f(1,1,1),v3f(1,0,0));
    cubeObj->addComponent(new Behavior(rotateObject));

    //Camera *camera = new OrthographicCamera(10, 0.5, 100);
    Camera *camera = new PerspectiveCamera(45, 4.0/3.0, 0.5, 100);
    camera->transform.position = v3f(6,6,6);
    //camera->addComponent(new Behavior(rotateObject));
    //TODO: right now camera is handled separately, should really generalize it to an ordinary object
    //also currently just looks at 0,0,0 from it's position (view is transformed with rotations/scaling)
    SceneManager::testScene.setCamera(camera);

    Object *light = Light::createPointLight(v3f(0,1,0));
    light->addChild(GeometricShape::createSphere(v3f(0,0,0), v3f::zero, v3f(0.1,0.1,0.1),v3f(1,1,0)));
    sphereObj->addChild(light);
    //SceneManager::testScene.addObject(light);
    SceneManager::testScene.addObject(planeObj);
    SceneManager::testScene.addObject(sphereObj);
    SceneManager::testScene.addObject(cubeObj);

    SceneManager::testScene.init();
    GraphicsSubsystem::run();

    return 0;
}


