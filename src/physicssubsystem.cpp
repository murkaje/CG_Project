#include "physicssubsystem.h"
#include <scenemanager.h>
#include <vec.h>
#include <math.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

void PhysicsSubsystem::PerformPhysicsChecks() {
    Scene* sceneObj = &SceneManager::CurrentScene();
    std::list<Object*> &objects = sceneObj->getObjsList();
    for (std::list<Object*>::iterator iterObj = objects.begin(); iterObj != objects.end(); iterObj++) {
        Collider *c = Collider::get(**iterObj);
        if (c != NULL) {
            c->collisions().clear();
            checkIntersections(**iterObj);
        }
    }
}

void PhysicsSubsystem::BoxToBoxIntersection(BoxCollider &collider, const BoxCollider &other) {
    Transform *t = Transform::get(*collider.owner());
    glm::vec3 Pos = t->position+collider.center;
    Transform *ot = Transform::get(*other.owner());

    glm::vec3 Diff = Pos - ot->position+other.center;
    glm::vec3 Distance = glm::vec3(fabs(Diff.x), fabs(Diff.y), fabs(Diff.z));
    glm::vec3 MinDistance = (collider.scale + other.scale)/2.f;

    float maxDistance = max(Distance.x, Distance.y);
    maxDistance = max(maxDistance, Distance.z);

    if(Distance.x < MinDistance.x && Distance.y < MinDistance.y && Distance.z < MinDistance.z) {
        //register collision
        collider.collisions().push_back(
            Collider::Collision(other, //collided with
                Pos-Distance+MinDistance, //collision point
                glm::vec3(int(Diff.x/maxDistance), int(Diff.y/maxDistance), int(Diff.z/maxDistance)) //normal of collision surface
            )
        );
    }
}

//Ray to oriented box intersection test
//First we construct the inverse transformation so the box becomes an axis-aligned cube (-1,-1,-1)...(1,1,1)
//Then we do a simple Ray to AABB intersection test
float PhysicsSubsystem::RayToBoxIntersection(const glm::vec3 &origin, const glm::vec3 &dir, BoxCollider &other) {
    Transform *t = Transform::get(*other.owner());
    glm::mat4 transMat(1);

    //Collision boxes with scale 1 are cubes -0.5...0.5
    transMat = glm::scale(transMat, glm::vec3(2.0)/t->scale);

    transMat = glm::rotate(transMat, -t->rotation.x, glm::vec3(1,0,0));
    transMat = glm::rotate(transMat, -t->rotation.y, glm::vec3(0,1,0));
    transMat = glm::rotate(transMat, -t->rotation.z, glm::vec3(0,0,1));

    transMat = glm::translate(transMat, -t->position);
    transMat = glm::translate(transMat, -other.center);

    glm::vec3 orig(transMat*glm::vec4(origin, 1.0));
    glm::vec3 dr(transMat*glm::vec4(dir, 0.0));

    //The box is now axis aligned, can perform Ray-AABB intersection
    glm::vec3 point = RayToAABBIntersection(orig, dr);
    point = glm::vec3(glm::inverse(transMat)*glm::vec4(point,1));

    return glm::length(point-origin);
}

//Detects ray intersection with axis-aligned bounding box (-1,-1,-1)...(1,1,1)
glm::vec3 PhysicsSubsystem::RayToAABBIntersection(const glm::vec3 &origin, const glm::vec3 &dir) {
    //If the ray origin is inside the cube, fail the raycast
    if( origin.x > -1 && origin.x < 1 &&
        origin.y > -1 && origin.y < 1 &&
        origin.z > -1 && origin.z < 1)
    {
        return glm::vec3(1.0/0.0, 1.0/0.0, 1.0/0.0);
    }

    glm::vec3 invDir = glm::vec3(1)/dir;
    glm::vec3 sign;
    sign.x = (invDir.x < 0 ? -1 : 1);
    sign.y = (invDir.y < 0 ? -1 : 1);
    sign.z = (invDir.z < 0 ? -1 : 1);

    glm::vec3 tmin = (-sign - origin) * invDir;
    glm::vec3 tmax = (sign - origin) * invDir;

    if(tmin.x <= tmax.y && tmin.y <= tmax.x &&
        tmin.y <= tmax.z && tmin.x <= tmax.z &&
        tmin.z <= tmax.y && tmin.z <= tmax.x)
    {
        glm::vec3 pmin = origin+std::max(std::max(tmin.x,tmin.y), tmin.z)*dir;
        glm::vec3 pmax = origin+std::min(std::min(tmax.x,tmax.y), tmax.z)*dir;

        return pmin;
    } else {
        return glm::vec3(1.0/0.0, 1.0/0.0, 1.0/0.0);
    }
}

float PhysicsSubsystem::RayToSphereIntersection(const glm::vec3 &origin, const glm::vec3 &dir, BoxCollider &other) {
    Transform *t = Transform::get(*other.owner());
    glm::mat4 transMat(1);

    transMat = glm::scale(transMat, glm::vec3(2.0) / t->scale);
    transMat = glm::translate(transMat, -t->position);
    transMat = glm::translate(transMat, -other.center);

    glm::vec3 orig(transMat*glm::vec4(origin, 1.0));
    glm::vec3 dr(transMat*glm::vec4(dir, 0.0));

    float a = glm::dot(dr, dr);
    float b = 2*glm::dot(dr, orig);
    float c = glm::dot(orig, orig)-1;

    if(b*b-4*a*c < 0) {
        return 1.0/0.0;
    } else {
        float q;
        if(b < 0) {
            q = (-b+sqrt(b*b-4*a*c))/2;
        } else {
            q = (-b-sqrt(b*b-4*a*c))/2;
        }

        if(q/a < c/q) {
            return q/a;
        } else {
            return c/q;
        }
    }
}

void PhysicsSubsystem::checkIntersections(Object &obj) {
    Scene* sceneObj = obj.getCurrentScene();
    Collider *oc = Collider::get(obj);
    std::list<Object*> &objects = sceneObj->getObjsList();
    for (std::list<Object*>::iterator iterObj = objects.begin(); iterObj != objects.end(); iterObj++) {
        if(!obj.equal(**iterObj)) {
            Collider *c = Collider::get(**iterObj);
            if (c != NULL) {
                switch (oc->typeId()) {
                case Collider::BOX:
                    switch (c->typeId()) {
                    case Collider::BOX:
                        BoxToBoxIntersection(*(BoxCollider*)Collider::get(obj), *(BoxCollider*)Collider::get(**iterObj));
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
