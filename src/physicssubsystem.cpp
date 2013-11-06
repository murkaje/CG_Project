#include "physicssubsystem.h"
#include <scenemanager.h>
#include <vector3f.h>
#include <math.h>
#include <iostream>

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

void PhysicsSubsystem::BoxToBoxIntersection(BoxCollider &collider, BoxCollider &other) {
    Transform *t = Transform::get(*collider.owner());
    float xPos = t->position.x+collider.center.x;
    float yPos = t->position.y+collider.center.y;
    float zPos = t->position.z+collider.center.z;

    Transform *ot = Transform::get(*other.owner());

    float xDistance = fabs(xPos - ot->position.x+other.center.x);
    float yDistance = fabs(yPos - ot->position.y+other.center.y);
    float zDistance = fabs(zPos - ot->position.z+other.center.z);

    float xMinDistance = (collider.scale.x + other.scale.x)/float(2);
    float yMinDistance = (collider.scale.y + other.scale.y)/float(2);
    float zMinDistance = (collider.scale.z + other.scale.z)/float(2);

    float maxDistance = max(xDistance, yDistance);
    maxDistance = max(maxDistance, zDistance);

    if(xDistance < xMinDistance && yDistance < yMinDistance && zDistance < zMinDistance) {
        //register collision
        collider.collisions().push_back(
            Collider::Collision(other, //collided with
                v3f(xPos-xDistance+xMinDistance, yPos-yDistance+yMinDistance, zPos-zDistance+zMinDistance), //collision point
                v3f(int(xDistance/maxDistance), int(yDistance/maxDistance), int(zDistance/maxDistance)) //normal of collision surface
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
                        BoxToBoxIntersection(*(BoxCollider*)Collider::get(obj), *(BoxCollider*)Collider::get(*iterObj));
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
