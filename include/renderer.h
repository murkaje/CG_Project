#ifndef INCLUDED_RENDERER_H
#define INCLUDED_RENDERER_H

#include <component.h>
#include <vec.h>
#include <material.h>

#include <utils.h>

extern bool NO_SHADER;

class Renderer: public Component {
protected:
    Renderer(int type=-1);
public:
    static const int MESH = 0;

    bool receive_shadows, cast_shadows;

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
    MeshRenderer(glm::vec3 color);

    void render();
};

template<> inline Component* Component::allocate_t<Renderer>(int type) {
    Component *newComp = NULL;
    if (type == Renderer::MESH) newComp = new MeshRenderer(glm::vec3(1));
    else printf("WARNING: COULD NOT ALLOCATE COMPONENT FOR TYPE_ID");
    return newComp;
};

#endif
