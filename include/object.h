#ifndef INCLUDED_OBJECT_H
#define INCLUDED_OBJECT_H

#include <string>
#include <map>
#include <list>

#include <transform.h>
#include <mesh.h>
#include <renderer.h>
#include <glm/glm.hpp>

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

    Object(const Object &other);


    void Synchronize(RakNet::BitStream *bs, RakNet::RPC3 *rpcFromNetwork = 0);
public:
    Object &obj;
    std::map<std::string,Component*> components;

    std::string name;
    std::string tag;

    static Object& copy(const Object &other);

    Object(std::string name="object");

    ~Object();

    void addComponent(Component *component);
    Component* getComponent(const std::string &name);

    Object* parent();

    virtual void registerWithScene(Scene *scene) {currentScene_ = scene;};
    virtual void unreisterWithScene(Scene *scene) {currentScene_ = NULL;};

    Scene* getCurrentScene();

    void addChild(Object &object);

    std::list<Object*>& getChildren();

    static Object* Find(const std::string &name);
    static Object* Find(Object* obj, const std::string &name);

    bool equal(const Object &other);

    friend class NetworkSubsystem;
};

class GeometricShape: public Object {
private:
    GeometricShape(Transform *transform, Mesh *mesh, MeshRenderer *renderer);
public:
    Transform &transform;
    Mesh &mesh;
    MeshRenderer &renderer;

    static GeometricShape* createPlane(const glm::vec3 &position,
                                        const glm::vec3 &rotation,
                                        const glm::vec3 &scale,
                                        const glm::vec3 &color);

    static GeometricShape* createSphere(const glm::vec3 &position,
                                        const glm::vec3 &rotation,
                                        const glm::vec3 &scale,
                                        const glm::vec3 &color);

    static GeometricShape* createCube(const glm::vec3 &position,
                                        const glm::vec3 &rotation,
                                        const glm::vec3 &scale,
                                        const glm::vec3 &color);

};

#endif
