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

void rotateObject(Object &obj) {
    int degreesPerSecond = 90;
    Transform *t = (Transform*)obj.getComponent(Component::TRANSFORM);
    t->rotation.z += degreesPerSecond*GraphicsSubsystem::delta;
}

int main(int argc, char* argv[]) {
    GraphicsSubsystem::init(argc,argv);
    GraphicsSubsystem::createWindow(30,30,640,480, "GraphicsProject2013");
    GraphicsSubsystem::zBufferEnabled(true);

    InputSubsystem::init();

    Object *planeObj = GeometricShape::createPlane(vector3f::zero,vector3f::zero,vector3f(6,6,6),vector3f(0.5,0.5,0.5));
    SceneManager::testScene.addObject(planeObj);

    Object *sphereObj = GeometricShape::createSphere(vector3f(0,1,0), vector3f::zero, vector3f(1,1,1),vector3f(0,1,0));
    SceneManager::testScene.addObject(sphereObj);

    Object *cubeObj = GeometricShape::createCube(vector3f(2,1,2), vector3f::zero, vector3f(1,1,1),vector3f(1,0,0));
    cubeObj->addComponent(new Behavior(rotateObject));
    SceneManager::testScene.addObject(cubeObj);

    Camera *camera = new OrthographicCamera(10, 0.5, 100);
    //Camera *camera = new PerspectiveCamera(45, 4.0/3.0, 0.5, 100);
    camera->transform.position = vector3f(6,6,6);
    //camera->addComponent(new Behavior(rotateObject));
    //TODO: right now camera is handled separately, should really generalize it to an ordinary object
    //also currently just looks at 0,0,0 from it's position (view is transformed with rotations/scaling)
    SceneManager::testScene.setCamera(camera);

    SceneManager::testScene.init();
    GraphicsSubsystem::run();

    return 0;
}


