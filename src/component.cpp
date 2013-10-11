
#include <component.h>
#include <object.h>

Component::Component(std::string name): name(name) {

}

const std::string Component::RENDERER   = "renderer";
const std::string Component::TRANSFORM  = "transform";
const std::string Component::MESH       = "mesh";
const std::string Component::BEHAVIOR   = "behavior";

Object* Component::owner() {
    return owner_;
}
