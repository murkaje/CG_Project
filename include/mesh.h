#ifndef INCLUDED_MESH_H
#define INCLUDED_MESH_H

#include <component.h>
#include <vector3f.h>

class Mesh: public Component {
private:

public:
    Mesh();
    virtual void describe() {};
};

class PlaneMesh: public Mesh {
public:
    void describe();
};

class SphereMesh: public Mesh {
public:
    void describe();
};

class CubeMesh: public Mesh {
public:
    void describe();
};

#endif
