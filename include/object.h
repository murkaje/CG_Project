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
#include "BitStream.h"

class Component;
class Scene;

class Object: public RakNet::NetworkIDObject {
private:
    static int nextNetworkId;

    Scene *currentScene_;

    std::list<Object*> children;
protected:
    Object* parent_;

    Object(Object &other);


    void Synchronize(RakNet::BitStream *bs, RakNet::RPC3 *rpcFromNetwork = 0);
public:
    Object &obj;
    std::map<std::string,Component*> components;

    std::string name;
    std::string tag;

    static Object& copy(Object &other);

    Object(std::string name="object");

    ~Object();

    void addComponent(Component *component);
    Component* getComponent(std::string name);

    Object* parent();

    virtual void registerWithScene(Scene *scene) {currentScene_ = scene;};
    virtual void unreisterWithScene(Scene *scene) {currentScene_ = NULL;};

    Scene* getCurrentScene();

    void addChild(Object &object);

    std::list<Object*>& getChildren();

    static Object* Find(std::string name);
    static Object* Find(Object* obj, std::string name);

    bool equal(Object &other);

    friend class NetworkSubsystem;
};

class GeometricShape: public Object {
private:
    GeometricShape(Transform *transform, Mesh *mesh, MeshRenderer *renderer);
public:
    Transform &transform;
    Mesh &mesh;
    MeshRenderer &renderer;

    static GeometricShape* createPlane(vec3f position, vec3f rotation, vec3f scale, vec3f color);
    static GeometricShape* createSphere(vec3f position, vec3f rotation, vec3f scale, vec3f color);
    static GeometricShape* createCube(vec3f position, vec3f rotation, vec3f scale, vec3f color);

};

#endif
