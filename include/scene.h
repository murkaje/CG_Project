#ifndef INCLUDED_SCENE_H
#define INCLUDED_SCENE_H

#include <list>
#include <object.h>
#include <glm/glm.hpp>

class Camera;

class Scene {
private:
    std::list<Object> objects;

    Camera *camera;
    glm::mat4 viewMat;

    void drawObjs(std::list<Object> &objects);
    void updateObjs(std::list<Object> &objects);

public:
    void init();
    void draw();
    void update();
    void addObject(Object *object);
    void setCamera(Camera *camera);
    void positionCamera();

    Object* raycastObject(const glm::vec2 &deviceCoords);
    Object* raycastObject(const glm::vec3 &origin, const glm::vec3 &dir);

    std::list<Object>& getObjsList();

    ~Scene();
};

#endif
