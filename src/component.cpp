
#include <component.h>
#include <object.h>
#include <cstdio>

Component::Component(std::string name): name(name) {

}

const std::string Component::RENDERER   = "renderer";
const std::string Component::TRANSFORM  = "transform";
const std::string Component::MESH       = "mesh";
const std::string Component::BEHAVIOR   = "behavior";
const std::string Component::LIGHT   = "light";
Object* Component::owner() {
    return owner_;
}

Component::~Component() {
    printf("destroyed component: %s of %s\n", name.c_str(), owner_->name.c_str());
}
