#ifndef INCLUDED_MESH_H
#define INCLUDED_MESH_H

#include <component.h>
#include <vector3f.h>

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

#endif
