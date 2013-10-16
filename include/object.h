#ifndef INCLUDED_OBJECT_H
#define INCLUDED_OBJECT_H

#include <string>
#include <map>

#include <transform.h>
#include <mesh.h>
#include <renderer.h>

class Component;
class Scene;

class Object {
private:
    Scene *currentScene_;
    std::map<std::string,Component*> components;

public:
    std::string name;

    Object(std::string name="object");

    ~Object();

    void addComponent(Component *component);
    Component* getComponent(std::string name);

    virtual void registerWithScene(Scene *scene) {currentScene_ = scene;};
    virtual void unreisterWithScene(Scene *scene) {currentScene_ = NULL;};

    Scene* getCurrentScene();
};

class GeometricShape: public Object {
private:
    GeometricShape(Transform *transform, Mesh *mesh, MeshRenderer *renderer);
public:
    Transform &transform;
    Mesh &mesh;
    MeshRenderer &renderer;

    static GeometricShape* createPlane(vector3f position, vector3f rotation, vector3f scale, vector3f color);
    static GeometricShape* createSphere(vector3f position, vector3f rotation, vector3f scale, vector3f color);
    static GeometricShape* createCube(vector3f position, vector3f rotation, vector3f scale, vector3f color);

};

#endif
