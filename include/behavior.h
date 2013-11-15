#ifndef INCLUDED_BEHAVIOR_H
#define INCLUDED_BEHAVIOR_H

#include <component.h>

#include <list>
#include <map>
#include <string>

typedef void (*behavior_function)(Object&);

class Behavior: public Component {
private:
    std::list<std::string> actions;

    static std::map<std::string, behavior_function> registeredFunctions;
protected:
    Behavior();
public:
    Behavior(std::string actionIdent, behavior_function action);
    Behavior(std::string actionIdent);

    static void add(Object* obj, std::string actionIdent, behavior_function action = NULL);

    static Behavior* get(Object &obj);

    static void Register(std::string actionIdent, behavior_function action);

    static void Execute(std::string actionIdent, Object& obj);

    void writeTo(RakNet::BitStream& out);
    void readFrom(RakNet::BitStream& in);

    friend class Component;
    friend class Scene;
};

#endif
