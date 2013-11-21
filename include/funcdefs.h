#ifndef FUNCDEFS_H_INCLUDED
#define FUNCDEFS_H_INCLUDED

#include <utils.h>
#include <vec.h>
#include <transform.h>
#include <renderer.h>
#include <graphicssubsystem.h>
#include <collider.h>
#include <eventmanager.h>
#include <synchronizer.h>
#include <networksubsystem.h>
#include <scenemanager.h>

#include "BitStream.h"

inline void moveLeft()
{
    SceneManager::moveVec.z() += SceneManager::movePerSecond * GraphicsSubsystem::delta;
}
inline void moveRight()
{
    SceneManager::moveVec.z() += -SceneManager::movePerSecond * GraphicsSubsystem::delta;
}
inline void moveBackward()
{
    SceneManager::moveVec.x() += SceneManager::movePerSecond * GraphicsSubsystem::delta;
}
inline void moveForward()
{
    SceneManager::moveVec.x() += -SceneManager::movePerSecond * GraphicsSubsystem::delta;
}

inline void rotateObject(Object &obj)
{
    int degreesPerSecond = 90;
    Transform::rotateObj(&obj, vec3f(0, 0, degreesPerSecond*GraphicsSubsystem::delta));
}

inline void moveObject(Object &obj)
{
    vec3f moveVec = SceneManager::moveVec;
    Collider *c = Collider::get(obj);
    std::list<Collider::Collision>::iterator col = c->collisions().begin();
    vec3f newPos = Transform::get(obj)->position+moveVec;
    for (; col != c->collisions().end(); col++) {
        //alter movement vector so we cannot go beyond collision points
        //printf("collision normal %f %f %f \n", col->normal.x, col->normal.y, col->normal.z);
        Transform *otherCol = Transform::get(*col->with.owner());
        if (otherCol->position.x() > newPos.x() && newPos.x() >= col->point.x()) moveVec.x() += moveVec.x()*col->normal.x(); //x+
        else if (otherCol->position.x() < newPos.x() && newPos.x() <= col->point.x()) moveVec.x() -= moveVec.x()*col->normal.x(); //x-
        if (otherCol->position.z() > newPos.z() && newPos.z() >= col->point.z()) moveVec.z() += moveVec.z()*col->normal.z(); //z+
        else if (otherCol->position.z() < newPos.z() && newPos.z() <= col->point.z()) moveVec.z() -= moveVec.z()*col->normal.z(); //z-
    }
    Transform::translateObj(&obj, moveVec);
    //reset movement vector for next frame
    moveVec = vec3f(0);
}

inline void colorCollidingObjects(Object &obj) {
    Collider *c = Collider::get(obj);
    std::list<Collider::Collision>::iterator col = c->collisions().begin();
    for (; col != c->collisions().end(); col++) {
        //printf("colliding with %s\n", col->with.owner()->name.c_str());
        Renderer::get(*col->with.owner())->material.diffuse = vec3f(0,1,0);
    }
}

inline void resetColorIfNoCollisions(Object &obj) {
    Collider *c = Collider::get(obj);
    if (c->collisions().size() == 0) Renderer::get(obj)->material.diffuse = vec3f(1,0,0);
}

inline void movementSynchronizer(Object& obj, RakNet::BitStream &bs, bool write) {
    vec3f moveVec = SceneManager::moveVec;
    Transform *t = Transform::get(obj);
    if (t != NULL) {
        if (write) {
            if (!NetworkSubsystem::isServer) {
                bs << moveVec;
                moveVec = vec3f(0);
            } else {
                bs << t->position;
            }
        } else {
            if (!NetworkSubsystem::isServer) {
                bs >> t->position;
            } else {
                bs >> moveVec;
            }
        }
    }
}

#endif
