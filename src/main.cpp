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
    Transform* t = (Transform*)obj.getComponent(Component::TRANSFORM);
    t->rotation.z += degreesPerSecond*GraphicsSubsystem::delta;
}

int main(int argc, char* argv[]) {
    GraphicsSubsystem::init(argc,argv);
    GraphicsSubsystem::createWindow(30,30,640,480, "GraphicsProject2013");
    GraphicsSubsystem::zBufferEnabled(true);

    InputSubsystem::init();

    Object *planeObj = new Object();
    planeObj->addComponent(new Transform(vector3f::zero, vector3f::zero, vector3f(6,6,6)));
    planeObj->addComponent(new PlaneMesh());
    //TODO: make a color class with predefined colors, also, when we introduce shaders the color should be
    //moved there from the renderer and the renderer would use the shader to render the mesh
    planeObj->addComponent(new MeshRenderer(vector3f(0.5,0.5,0.5)));
    planeObj->addComponent(new Behavior(rotateObject));

    SceneManager::testScene.addObject(planeObj);

    Camera *camera = new OrthographicCamera(10, 0.5, 100);
    //Camera *camera = new PerspectiveCamera(45, 4.0/3.0, 0.5, 100);
    camera->transform.position = vector3f(6,6,6);
    camera->addComponent(new Behavior(rotateObject));
    //TODO: right now camera is handled separately, should really generalize it to an ordinary object
    //also currently just looks at 0,0,0 from it's position (view is transformed with rotations/scaling)
    SceneManager::testScene.setCamera(camera);

    SceneManager::testScene.init();
    GraphicsSubsystem::run();
    return 0;
}


