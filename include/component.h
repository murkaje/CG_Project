#ifndef INCLUDED_COMPONENT_H
#define INCLUDED_COMPONENT_H

#include <string>

class Object;

class Component {
private:

protected:
    Object *owner_;
public:
    static const std::string RENDERER;
    static const std::string TRANSFORM;
    static const std::string MESH;

    Component(std::string name);

    std::string name;

    Object* owner();

    friend class Object;
};

#endif
