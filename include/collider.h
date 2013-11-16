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
        v3f point, normal;

        Collision(Collider &with, v3f point, v3f normal);
    };

    std::list<Collision> _collisions;

    Collider(int type=0);

public:
    static const int BOX = 0;
    static const int SPHERE = 1;

    std::list<Collision>& collisions();

    static void addBox(Object &obj);

    static Collider* get(Object &obj);

    friend class PhysicsSubsystem;
    friend class Component;
};

class BoxCollider: public Collider {
public:
    v3f center, rotation, scale;

    void writeTo(RakNet::BitStream& out);
    void readFrom(RakNet::BitStream& in);

    BoxCollider();
};

#endif
