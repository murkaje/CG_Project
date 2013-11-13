#ifndef INCLUDED_OBJECT_H
#define INCLUDED_OBJECT_H

#include <string>
#include <map>
#include <list>

#include <transform.h>
#include <mesh.h>
#include <renderer.h>
#include <string>

#include "NetworkIDObject.h"

class Component;
class Scene;

class Object: public RakNet::NetworkIDObject {
private:
    static int nextNetworkId;

    Scene *currentScene_;
    std::map<std::string,Component*> components;

    RakNet::NetworkID nid;

    std::list<Object> children;
protected:
    Object* parent_;
public:
    Object &obj;

    std::string name;

    Object(std::string name="object");

    ~Object();

    void addComponent(Component *component);
    Component* getComponent(std::string name);

    Object* parent();

    virtual void registerWithScene(Scene *scene) {currentScene_ = scene;};
    virtual void unreisterWithScene(Scene *scene) {currentScene_ = NULL;};

    Scene* getCurrentScene();

    void addChild(Object *object);

    std::list<Object>& getChildren();

    static Object* Find(std::string name);
    static Object* Find(Object* obj, std::string name);

    bool equal(Object &other);
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
