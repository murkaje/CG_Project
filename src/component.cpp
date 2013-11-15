
#include <component.h>
#include <object.h>
#include <cstdio>

#include <renderer.h>
#include <transform.h>
#include <mesh.h>
#include <behavior.h>
#include <light.h>
#include <camera.h>
#include <collider.h>
#include <synchronizer.h>

Component::Component(std::string name): type(-1), name(name) {

}

const std::string Component::RENDERER       = "renderer";
const std::string Component::TRANSFORM      = "transform";
const std::string Component::MESH           = "mesh";
const std::string Component::BEHAVIOR       = "behavior";
const std::string Component::LIGHT          = "light";
const std::string Component::CAMERA         = "camera";
const std::string Component::COLLIDER       = "collider";
const std::string Component::SYNCHRONIZER   = "synchronizer";

Component* Component::allocate(std::string compName, int type) {
    if (compName.compare(Component::RENDERER) == 0) {
        return allocate_t<Renderer>(type);
    } else if (compName.compare(Component::TRANSFORM) == 0) {
        return allocate_t<Transform>(type);
    } else if (compName.compare(Component::MESH) == 0) {
        return allocate_t<Mesh>(type);
    } else if (compName.compare(Component::BEHAVIOR) == 0) {
        return allocate_t<Behavior>(type);
    } else if (compName.compare(Component::LIGHT) == 0) {
        return allocate_t<Light>(type);
    } else if (compName.compare(Component::CAMERA) == 0) {
        return allocate_t<Camera>(type);
    } else if (compName.compare(Component::COLLIDER) == 0) {
        return allocate_t<Collider>(type);
    } else if (compName.compare(Component::SYNCHRONIZER) == 0) {
        return allocate_t<Synchronizer>(type);
    } else {
        printf("No way to allocate '%s'", compName.c_str());
        return NULL;
    }
}

Object* Component::owner() {
    return owner_;
}


int Component::typeId() {
    return type;
}

Component::~Component() {
    printf("destroyed component: %s of %s\n", name.c_str(), owner_->name.c_str());
}
