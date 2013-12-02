#ifndef INCLUDED_MESH_H
#define INCLUDED_MESH_H

#include <component.h>
#include <vec.h>

class Mesh: public Component {
protected:
    Mesh(int type=-1);
public:
    static const int PLANE  = 0;
    static const int SPHERE = 1;
    static const int CUBE   = 2;
    static const int MODEL  = 3;

    virtual void describe() {};

    void writeTo(RakNet::BitStream& out);
    void readFrom(RakNet::BitStream& in);

    static Mesh* get(Object &obj);

    friend class Component;
};

class PlaneMesh: public Mesh {
public:
    PlaneMesh();
    void describe();
};

class SphereMesh: public Mesh {
public:
    SphereMesh();
    void describe();
};

class CubeMesh: public Mesh {
public:
    CubeMesh();
    void describe();
};

template<> inline Component* Component::allocate_t<Mesh>(int type) {
    Component *newComp = NULL;
    if (type == Mesh::PLANE) newComp = new PlaneMesh();
    else if (type == Mesh::CUBE) newComp = new CubeMesh();
    else if (type == Mesh::SPHERE) newComp = new SphereMesh();
    else printf("WARNING: COULD NOT ALLOCATE COMPONENT FOR TYPE_ID");
    return newComp;
};

#endif
