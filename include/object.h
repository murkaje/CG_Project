#ifndef INCLUDED_OBJECT_H
#define INCLUDED_OBJECT_H

#include <string>
#include <map>

class Component;

class Object {
private:
    std::map<std::string,Component*> components;

public:

    ~Object();

    void addComponent(Component *component);
    Component* getComponent(std::string name);
};

#endif
