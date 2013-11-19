#include "physicssubsystem.h"
#include <scenemanager.h>
#include <vec.h>
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
//    float xPos = t->position.x()+collider.center.x();
//    float yPos = t->position.y()+collider.center.y();
//    float zPos = t->position.z()+collider.center.z();
    vec3f Pos = t->position+collider.center;

    Transform *ot = Transform::get(*other.owner());

//    float xDiff = xPos - ot->position.x()+other.center.x();
//    float yDiff = yPos - ot->position.y()+other.center.y();
//    float zDiff = zPos - ot->position.z()+other.center.z();
    vec3f Diff = Pos - ot->position+other.center;

//    float xDistance = fabs(xDiff);
//    float yDistance = fabs(yDiff);
//    float zDistance = fabs(zDiff);
    vec3f Distance = vec3f(fabs(Diff.x()), fabs(Diff.y()), fabs(Diff.z()));

//    float xMinDistance = (collider.scale.x() + other.scale.x())/float(2);
//    float yMinDistance = (collider.scale.y() + other.scale.y())/float(2);
//    float zMinDistance = (collider.scale.z() + other.scale.z())/float(2);
    vec3f MinDistance = (collider.scale + other.scale)/2;

    float maxDistance = max(Distance.x(), Distance.y());
    maxDistance = max(maxDistance, Distance.z());

    if(Distance.x() < MinDistance.x() && Distance.y() < MinDistance.y() && Distance.z() < MinDistance.z()) {
        //register collision
        collider.collisions().push_back(
            Collider::Collision(other, //collided with
                //vec3f(Pos.x()-Distance.x()+MinDistance.x(), Pos.y()-Distance.y()+MinDistance.y(), Pos.z()-Distance.z()+MinDistance.z()), //collision point
                Pos-Distance+MinDistance, //collision point
                vec3f(int(Diff.x()/maxDistance), int(Diff.y()/maxDistance), int(Diff.z()/maxDistance)) //normal of collision surface
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
