
#include <object.h>
#include <component.h>

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

Object::~Object() {
    for (std::map<std::string,Component*>::iterator comp = components.begin(); comp != components.end(); comp++) {
        delete comp->second;
    }
}
