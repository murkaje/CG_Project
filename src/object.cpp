
#include <object.h>
#include <component.h>
#include <scenemanager.h>
#include <networksubsystem.h>
#include <synchronizer.h>

#include <cstdio>

int Object::nextNetworkId = 0;

Object* find_(std::list<Object*> &objects, const std::string &name) {
    for (std::list<Object*>::iterator obj = objects.begin(); obj != objects.end(); obj++) {
        if ((*obj)->name.compare(name) == 0) {
            return (*obj);
        }
        Object *fromChild = find_((*obj)->getChildren(), name);
        if (fromChild != NULL) return fromChild;
    }
    return NULL;
}

Object* Object::Find(const std::string &name) {
    return find_(SceneManager::CurrentScene().getObjsList(), name);
}

Object* Object::Find(Object* obj, const std::string &name) {
    if (obj->name.compare(name) == 0) return obj;
    else return find_(obj->getChildren(), name);
}

bool Object::equal(const Object &other) {
    return &other == this;
}

Object::Object(std::string name): parent_(NULL), obj(*this), name(name) {
    SetNetworkIDManager(&NetworkSubsystem::networkIDManager);
    if (NetworkSubsystem::isServer) {
        SetNetworkID(nextNetworkId++);
    }
}

void Object::Synchronize(RakNet::BitStream *bs, RakNet::RPC3 *rpcFromNetwork) {
    if (rpcFromNetwork == 0) {
        Synchronizer *s = Synchronizer::get(*this);
        s->write(*bs);
        NetworkSubsystem::rpc->CallCPP("&Object::Synchronize", GetNetworkID(), bs);
    } else {
        Synchronizer *s = Synchronizer::get(*this);
        s->read(*bs);
    }
}


Object::Object(const Object &other): parent_(other.parent()), obj(*this), name(other.name), tag(other.tag) {
    SetNetworkIDManager(other.networkIDManager);
	SetNetworkID(other.GetNetworkID());

	components = other.components;

	for (std::map<std::string,Component*>::iterator comp = components.begin(); comp != components.end(); comp++) {
        if (comp->second != NULL) {
            comp->second->owner_ = this;
        }
    }
	for (std::list<Object*>::iterator obj = other.getChildren().begin(); obj != other.getChildren().end(); obj++) {
        addChild(copy(**obj));
    }
}

Object& Object::copy(const Object &other) {
    return *(new Object(other));
}

void Object::addComponent(Component *component) {
    std::map<std::string,Component*>::iterator comp = components.find(component->name);
    if (comp != components.end())
        delete comp->second;
    component->owner_ = this;
    components.insert(std::pair<std::string,Component*>(component->name, component));
}

Component* Object::getComponent(const std::string &name) {
    std::map<std::string,Component*>::iterator comp = components.find(name);
    if (comp == components.end())
        return NULL;
    else
        return components[name];
}

Object* Object::parent() {
    return parent_;
}

Scene* Object::getCurrentScene() {
    return currentScene_;
}

void Object::addChild(Object& object) {
    object.parent_ = this;
    children.push_back(&object);
}

std::list<Object*>& Object::getChildren() {
    return children;
}

Object::~Object() {
    //disabled temporarily until copying of components is implemented for Object::copy?
    /*for (std::map<std::string,Component*>::iterator comp = components.begin(); comp != components.end(); comp++) {
        if (comp->second != NULL) {
            delete comp->second;
            comp->second = NULL;
        }
    }*/
    for (std::list<Object*>::iterator obj = getChildren().begin(); obj != getChildren().end(); obj++) {
        if (*obj != NULL) {
            delete *obj;
            (*obj) = NULL;
        }
    }
    printf("destroyed object: %s\n", name.c_str());
}

GeometricShape::GeometricShape(Transform *transform, Mesh *mesh, MeshRenderer *renderer): transform(*transform), mesh(*mesh), renderer(*renderer) {
    addComponent(transform);
    addComponent(mesh);
    addComponent(renderer);
    name = "geometricObject";
}

GeometricShape* GeometricShape::createPlane(const glm::vec3 &position,
                                            const glm::vec3 &rotation,
                                            const glm::vec3 &scale,
                                            const glm::vec3 &color)
{
    GeometricShape *obj = new GeometricShape(new Transform(position, rotation, scale),
                                               new PlaneMesh(),
                                               new MeshRenderer(color));
    obj->name = "plane";
    return obj;
}

GeometricShape* GeometricShape::createSphere(const glm::vec3 &position,
                                            const glm::vec3 &rotation,
                                            const glm::vec3 &scale,
                                            const glm::vec3 &color)
{
    GeometricShape *obj = new GeometricShape(new Transform(position, rotation, scale),
                                               new SphereMesh(),
                                               new MeshRenderer(color));
    obj->name = "sphere";
    return obj;
}

GeometricShape* GeometricShape::createCube(const glm::vec3 &position,
                                            const glm::vec3 &rotation,
                                            const glm::vec3 &scale,
                                            const glm::vec3 &color)
{
    GeometricShape *obj = new GeometricShape(new Transform(position, rotation, scale),
                                               new CubeMesh(),
                                               new MeshRenderer(color));
    obj->name = "cube";
    return obj;
}
