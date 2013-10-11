#ifndef INCLUDED_SCENE_H
#define INCLUDED_SCENE_H

#include <list>
#include <object.h>

class Scene {
private:
    std::list<Object> objects;

public:
    void draw();

    void addObject(Object *object);
};

#endif
