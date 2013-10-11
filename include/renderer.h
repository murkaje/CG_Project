#ifndef INCLUDED_RENDERER_H
#define INCLUDED_RENDERER_H

#include <component.h>
#include <vector3f.h>

class Renderer: public Component {
private:

public:

    Renderer();

    virtual void render() {};
};

class MeshRenderer: public Renderer {
private:

public:
    vector3f color;

    MeshRenderer(vector3f color);

    void render();
};

#endif
