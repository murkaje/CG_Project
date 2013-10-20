
#include <object.h>
#include <component.h>
#include <scenemanager.h>

#include <cstdio>

Object* find_(std::list<Object> &objects, std::string name) {
    for (std::list<Object>::iterator obj = objects.begin(); obj != objects.end(); obj++) {
        if ((*obj).name.compare(name) == 0) {
            return &(*obj);
        }
        Object *fromChild = find_((*obj).getChildren(), name);
        if (fromChild != NULL) return fromChild;
    }
    return NULL;
}

Object* Object::Find(std::string name) {
    return find_(SceneManager::CurrentScene().getObjsList(), name);
}

Object* Object::Find(Object* obj, std::string name) {
    if (obj->name.compare(name) == 0) return obj;
    else return find_(obj->getChildren(), name);
}

Object::Object(std::string name): parent_(NULL), name(name) {

}

void Object::addComponent(Component *component) {
    std::map<std::string,Component*>::iterator comp = components.find(component->name);
    if (comp != components.end())
        delete comp->second;
    component->owner_ = this;
    components.insert(std::pair<std::string,Component*>(component->name, component));
}

Component* Object::getComponent(std::string name) {
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

void Object::addChild(Object* object) {
    object->parent_ = this;
    children.push_back(*object);
}

std::list<Object>& Object::getChildren() {
    return children;
}

Object::~Object() {
    for (std::map<std::string,Component*>::iterator comp = components.begin(); comp != components.end(); comp++) {
        if (comp->second != NULL) {
            delete comp->second;
            comp->second = NULL;
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

GeometricShape* GeometricShape::createPlane(vector3f position, vector3f rotation, vector3f scale, vector3f color) {
    GeometricShape *obj = new GeometricShape(new Transform(position, rotation, scale),
                                               new PlaneMesh(),
                                               new MeshRenderer(color));
    obj->name = "plane";
    return obj;
}

GeometricShape* GeometricShape::createSphere(vector3f position, vector3f rotation, vector3f scale, vector3f color) {
    GeometricShape *obj = new GeometricShape(new Transform(position, rotation, scale),
                                               new SphereMesh(),
                                               new MeshRenderer(color));
    obj->name = "sphere";
    return obj;
}

GeometricShape* GeometricShape::createCube(vector3f position, vector3f rotation, vector3f scale, vector3f color) {
    GeometricShape *obj = new GeometricShape(new Transform(position, rotation, scale),
                                               new CubeMesh(),
                                               new MeshRenderer(color));
    obj->name = "cube";
    return obj;
}
