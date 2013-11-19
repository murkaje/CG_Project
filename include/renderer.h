#ifndef INCLUDED_RENDERER_H
#define INCLUDED_RENDERER_H

#include <component.h>
#include <vec.h>
#include <material.h>

#include <utils.h>

class Renderer: public Component {
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
    MeshRenderer(vec3f color);

    void render();
};

template<> inline Component* Component::allocate_t<Renderer>(int type) {
    Component *newComp = NULL;
    if (type == Renderer::MESH) newComp = new MeshRenderer(vec3f(1));
    else printf("WARNING: COULD NOT ALLOCATE COMPONENT FOR TYPE_ID");
    return newComp;
};

#endif
