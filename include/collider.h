#ifndef COLLIDER_H
#define COLLIDER_H

#include <component.h>
#include <object.h>
#include <utils.h>

#include <list>

class Collider: public Component {
protected:
    struct Collision {
        Collider &with;
        vec3f point, normal;

        Collision(Collider &with, vec3f point, vec3f normal);
    };

    int _type;
    std::list<Collision> _collisions;

    Collider(int type);

public:
    static const int BOX = 0;
    static const int SPHERE = 1;

    std::list<Collision>& collisions();

    int type();

    static void addBox(Object &obj);

    static Collider* get(Object &obj);

    friend class PhysicsSubsystem;
};

class BoxCollider: public Collider {
public:
    vec3f center, rotation, scale;

    BoxCollider();
};

#endif
