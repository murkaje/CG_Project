#ifndef INCLUDED_RENDERER_H
#define INCLUDED_RENDERER_H

#include <component.h>
#include <vector3f.h>
#include <material.h>

#include <utils.h>

class Renderer: public Component {
private:
    int type;
protected:
    Renderer(int type=-1);
public:
    static const int MESH = 0;

    Material material;

    virtual void render() {};

    void writeTo(RakNet::BitStream& out);
    void readFrom(RakNet::BitStream& in);

    static Renderer* get(Object &obj);

    friend class Component;
};

class MeshRenderer: public Renderer {
private:

public:
    MeshRenderer(vector3f color);

    void render();
};

#endif
