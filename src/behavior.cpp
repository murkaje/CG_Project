#include <object.h>
#include <behavior.h>

std::map<std::string, behavior_function> Behavior::registeredFunctions;

void Behavior::writeTo(RakNet::BitStream& out) {
    unsigned int numIdents = actions.size();
    out.Write(numIdents);
    std::list<std::string>::iterator ident = actions.begin();
    for (; ident != actions.end(); ident++) {
        RakNet::RakString rs(ident->c_str());
        out.Write(rs);
    }
}

void Behavior::readFrom(RakNet::BitStream& in) {
    unsigned int numIdents;
    in.Read(numIdents);
    for (unsigned int i = 0; i < numIdents; i++) {
        RakNet::RakString rs;
        in.Read(rs);
        actions.push_back(rs.C_String());
    }
}

Behavior::Behavior(): Component(Component::BEHAVIOR) {

}

Behavior::Behavior(std::string actionIdent, behavior_function action, bool local): Component(Component::BEHAVIOR) {
    Register(actionIdent, action);
    if (local)
        actionsLocal.push_back(actionIdent);
    else
        actions.push_back(actionIdent);
}

Behavior::Behavior(std::string actionIdent, bool local): Component(Component::BEHAVIOR) {
    if (local)
        actionsLocal.push_back(actionIdent);
    else
        actions.push_back(actionIdent);
}

Behavior* Behavior::get(Object &obj) {
    return (Behavior*)obj.getComponent(Component::BEHAVIOR);
}

void Behavior::add(Object* obj, std::string actionIdent, behavior_function action) {
    Behavior *behavior = (Behavior*)obj->getComponent(Component::BEHAVIOR);
    if (action != NULL) {
        Register(actionIdent, action);
    }
    if (behavior == NULL) {
        behavior = new Behavior(actionIdent);
        obj->addComponent(behavior);
    } else {
        behavior->actions.push_back(actionIdent);
    }
}

void Behavior::addLocal(Object* obj, std::string actionIdent, behavior_function action) {
    Behavior *behavior = (Behavior*)obj->getComponent(Component::BEHAVIOR);
    if (action != NULL) {
        Register(actionIdent, action);
    }
    if (behavior == NULL) {
        behavior = new Behavior(actionIdent, true);
        obj->addComponent(behavior);
    } else {
        behavior->actionsLocal.push_back(actionIdent);
    }
}

void Behavior::Register(std::string actionIdent, behavior_function action) {
    std::map<std::string,behavior_function>::iterator func = registeredFunctions.find(actionIdent);
    if (func != registeredFunctions.end())
        printf("Overriding previously registered behavior.\n");
    registeredFunctions.insert(std::pair<std::string,behavior_function>(actionIdent, action));
}

void Behavior::Execute(std::string actionIdent, Object& obj) {
    std::map<std::string,behavior_function>::iterator func = registeredFunctions.find(actionIdent);
    if (func == registeredFunctions.end())
        printf("Attempted to execute unregistered behavior '%s'\n", actionIdent.c_str());
    else
        (func->second)(obj);
}
