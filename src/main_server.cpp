#include <utils.h>
#include <scene.h>
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

#include "funcdefs.h"

int main(int argc, char* argv[])
{

    NetworkSubsystem::init();
    NetworkSubsystem::startServer();

    GraphicsSubsystem::init(argc,argv);
    GraphicsSubsystem::createWindow(30,30,30,30, "GraphicsProject2013Server");

    //InputSubsystem::init();

    //The plane "quad mesh" doesn't really work with lighting properly, super thin cube for now instead :(
    //Object *planeObj = GeometricShape::createPlane(vec3f::zero,vec3f::zero,vec3f(6,0,6),vec3f(0.5,0.5,0.5));
    Object *planeObj = GeometricShape::createCube(vec3f(0,0,0),vec3f(0,0,0), vec3f(25,0.001,25),vec3f(0.5,0.5,0.5));
    planeObj->name = "plane";
    //make the plane really shiny
    Renderer::get(*planeObj)->material.specular = vec3f(1);
    Renderer::get(*planeObj)->material.shininess = 100;

    Object *sphereObj = GeometricShape::createSphere(vec3f(0,0.5,0), vec3f(0,45,0), vec3f(1,1,1),vec3f(0,1,0));
    Behavior::addLocal(sphereObj, "moveObject", moveObject);
    Collider::addBox(*sphereObj);
    Behavior::add(sphereObj, "colorCollidingObjects", colorCollidingObjects);
    //Synchronizer::add(sphereObj, objSaysHello);
    Synchronizer::add(sphereObj, "movementSynchronizer", movementSynchronizer);

    Object *cubeObj = GeometricShape::createCube(vec3f(2,1,2), vec3f(0), vec3f(1,1,1),vec3f(1,0,0));
    Behavior::add(cubeObj, "rotateObject", rotateObject);
    Collider::addBox(*cubeObj);
    Behavior::add(cubeObj, "resetColorIfNoCollisions", resetColorIfNoCollisions);
    //Synchronizer::add(cubeObj, objSaysHello);

    Object *secondCubeObj = GeometricShape::createCube(vec3f(2,0.5,0), vec3f(0), vec3f(3.5,.5,.5),vec3f(1,0,1));
    Collider::addBox(*secondCubeObj);
    Behavior::add(secondCubeObj, "resetColorIfNoCollisions");

    //Object *camera = Camera::createOrthographicCamera(10, 0.5, 100);
    Object *camera = Camera::createPerspectiveCamera(45, 4.0/3.0, 0.5, 100);
    Transform::setObjPosition(camera, vec3f(0, 6, 6));
    Transform::setObjRotation(camera, vec3f(-45, 0, 0));
    camera->name = "MainCamera";
    sphereObj->addChild(*camera);

    Object *light = Light::createPointLight(vec3f(0,1,0));
    Object *lightBall = GeometricShape::createSphere(vec3f(0), vec3f(0), vec3f(0.1,0.1,0.1),vec3f(1,1,0));
    Renderer::get(*lightBall)->material.lighting_enabled = false;
    light->addChild(*lightBall);
    sphereObj->addChild(*light);

    Instantiate(*planeObj);
    Instantiate(*sphereObj);
    Instantiate(*cubeObj);
    Instantiate(*secondCubeObj);

    GraphicsSubsystem::run();

    NetworkSubsystem::shutdown();

    printf("Successful termination");
    return 0;
}


