#ifndef INCLUDED_SCENE_H
#define INCLUDED_SCENE_H

#include <list>
#include <object.h>

class Camera;

class Scene {
private:
    std::list<Object> objects;

    Camera *camera;

    void drawObjs(std::list<Object> &objects);
    void updateObjs(std::list<Object> &objects);

public:
    void init();

    void draw();

    void update();

    void addObject(Object *object);

    void setCamera(Camera *camera);

    ~Scene();
};

#endif
