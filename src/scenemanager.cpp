#include "scenemanager.h"
#include "object.h"
#include "component.h"
#include "collider.h"
#include "renderer.h"
#include "behavior.h"
#include "transform.h"
#include "synchronizer.h"
#include "light.h"
#include "camera.h"

#include "funcdefs.h"

Scene SceneManager::testScene = Scene();

Scene& SceneManager::CurrentScene() {
    return testScene;
}

void SceneManager::createTestScene() {
    //The plane "quad mesh" doesn't really work with lighting properly, super thin cube for now instead :(
    //Object *planeObj = GeometricShape::createPlane(vec3f::zero,vec3f::zero,vec3f(6,0,6),vec3f(0.5,0.5,0.5));
    Object *planeObj = GeometricShape::createCube(vec3f(0,0,0),vec3f(0,0,0), vec3f(25,0.001,25),vec3f(0.5,0.5,0.5));
    planeObj->name = "plane";
    //make the plane really shiny
    Renderer::get(*planeObj)->material.specular = vec3f(1);
    Renderer::get(*planeObj)->material.shininess = 100;

    Object *cubeObj = GeometricShape::createCube(vec3f(2,1,2), vec3f(0), vec3f(1,1,1),vec3f(1,0,0));
    Behavior::addLocal(cubeObj, "rotateObject", rotateObject);
    Collider::addBox(*cubeObj);
    Behavior::addLocal(cubeObj, "resetColorIfNoCollisions", resetColorIfNoCollisions);
    //Synchronizer::add(cubeObj, objSaysHello);

    Object *secondCubeObj = GeometricShape::createCube(vec3f(2,0.5,0), vec3f(0), vec3f(3.5,.5,.5),vec3f(1,0,1));
    Collider::addBox(*secondCubeObj);
    Behavior::addLocal(secondCubeObj, "resetColorIfNoCollisions");

    Instantiate(*planeObj);
    Instantiate(*cubeObj);
    Instantiate(*secondCubeObj);

    Synchronizer::Register("movementSynchronizer", movementSynchronizer);
    Behavior::Register("colorCollidingObjects", colorCollidingObjects);
}
