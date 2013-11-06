#include "physicssubsystem.h"
#include <scenemanager.h>
#include <vector3f.h>
#include <math.h>
#include <iostream>
#include "collider.h"

using namespace std;

void PhysicsSubsystem::PerformPhysicsChecks() {
    Scene* sceneObj = &SceneManager::CurrentScene();
    std::list<Object> &objects = sceneObj->getObjsList();
    for (std::list<Object>::iterator iterObj = objects.begin(); iterObj != objects.end(); iterObj++) {
        Collider *c = Collider::get(*iterObj);
        if (c != NULL) {
            c->collisions().clear();
            checkIntersections(*iterObj);
        }
    }
}

void PhysicsSubsystem::checkBoxToBoxIntersection(Object &obj, Object &iterObj) {
    Transform &objT = *Transform::get(obj);
    Transform &iterObjT = *Transform::get(iterObj);
//  float euclideanDistanceBetweenObjects = sqrt(pow((objT.position.x - iterObjT.position.x),2) +
//                                                    pow((objT.position.y - iterObjT.position.y),2) +
//                                                    pow((objT.position.z - iterObjT.position.z),2));

    float xDistance = fabs(objT.position.x - iterObjT.position.x);
    float yDistance = fabs(objT.position.y - iterObjT.position.y);
    float zDistance = fabs(objT.position.z - iterObjT.position.z);

    float xMinDistance = (objT.scale.x + iterObjT.scale.x)/float(2);
    float yMinDistance = (objT.scale.y + iterObjT.scale.y)/float(2);
    float zMinDistance = (objT.scale.z + iterObjT.scale.z)/float(2);

    float m = max(xDistance, yDistance);
    m = max(m, zDistance);

//  cout<<xDistance<<" x min: "<<xMinDistance<<endl;
//  cout<<yDistance<<" y min: "<<yMinDistance<<endl;
//  cout<<zDistance<<" z min: "<<zMinDistance<<endl;
    if(xDistance < xMinDistance && yDistance < yMinDistance && zDistance < zMinDistance) {
        //add collision object with reference to colliding collider and a collision point
        Collider::get(obj)->collisions().push_back(
            Collider::Collision(
                *Collider::get(iterObj),
                v3f(objT.position.x-xDistance+xMinDistance,objT.position.y-yDistance+yMinDistance,objT.position.z-zDistance+zMinDistance),
                v3f(xDistance/m == 1.0 ? xDistance/m : 0, yDistance/m == 1.0 ? yDistance/m : 0, zDistance/m == 1.0 ? zDistance/m : 0)
            )
        );
    }
}

void PhysicsSubsystem::checkIntersections(Object &obj) {
    Scene* sceneObj = obj.getCurrentScene();
    Collider *oc = Collider::get(obj);
    std::list<Object> &objects = sceneObj->getObjsList();
    for (std::list<Object>::iterator iterObj = objects.begin(); iterObj != objects.end(); iterObj++) {
        if(!obj.equal(*iterObj)) {
            Collider *c = Collider::get(*iterObj);
            if (c != NULL) {
                switch (oc->type()) {
                case Collider::BOX:
                    switch (c->type()) {
                    case Collider::BOX:
                        checkBoxToBoxIntersection(obj, *iterObj);
                        break;
                    case Collider::SPHERE:
                        break;
                    default:
                        break;
                    }
                    break;
                case Collider::SPHERE:
                    break;
                default:
                    break;
                }
            }

        }
    }
}
