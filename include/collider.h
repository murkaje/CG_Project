#ifndef COLLIDER_H
#define COLLIDER_H

#include <component.h>
#include <object.h>
#include <utils.h>

#include <list>

class Collider: public Component {
protected:
    struct Collision {
        const Collider &with;
        glm::vec3 point, normal;

        Collision(const Collider &with, const glm::vec3 &point, const glm::vec3 &normal);
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
    glm::vec3 center, rotation, scale;

    void writeTo(RakNet::BitStream& out);
    void readFrom(RakNet::BitStream& in);

    BoxCollider();
};

template<> inline Component* Component::allocate_t<Collider>(int type) {
    Component *newComp = NULL;
    if (type == Collider::BOX) newComp = new BoxCollider();
    else printf("WARNING: COULD NOT ALLOCATE COMPONENT FOR TYPE_ID");
    return newComp;
};

#endif
