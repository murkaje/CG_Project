#ifndef INCLUDED_COMPONENT_H
#define INCLUDED_COMPONENT_H

#include <string>

#include "BitStream.h"

class Object;

class Component {
private:
protected:
    int type;
    Object *owner_;

    template<class T> static Component* allocate_t(int type);
public:
    static const std::string RENDERER;
    static const std::string TRANSFORM;
    static const std::string MESH;
    static const std::string BEHAVIOR;
    static const std::string LIGHT;
    static const std::string CAMERA;
    static const std::string COLLIDER;
    static const std::string SYNCHRONIZER;

    Component(std::string name="component");

    ~Component();


    int typeId();

    std::string name;

    Object* owner();

    virtual void writeTo(RakNet::BitStream& out) {};
    virtual void readFrom(RakNet::BitStream& in) {};

    static Component* allocate(std::string, int type=0);

    friend class Object;
    friend class NetworkSubsystem;
};

//use template specialization to override the default behavior for specific Components
template<class T> Component* Component::allocate_t(int type) {
    Component *newComp = new T();
    return newComp;
};

#endif
