#ifndef INCLUDED_RENDERER_H
#define INCLUDED_RENDERER_H

#include <component.h>
#include <vec.h>

#include <utils.h>
#include <material.h>

class Renderer: public Component {

public:
    Material material;

    Renderer();

    virtual void render() {};

    static Renderer* get(Object &obj);
};

class MeshRenderer: public Renderer {
private:

public:
    MeshRenderer(vec3f color);

    void render();
};

#endif
