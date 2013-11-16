#include "synchronizer.h"
#include <object.h>

std::map<std::string, sync_function> Synchronizer::registeredFunctions;

Synchronizer::Synchronizer(): Component(Component::SYNCHRONIZER) {

}

Synchronizer::Synchronizer(std::string syncIdent, sync_function sync): Component(Component::SYNCHRONIZER) {
    Register(syncIdent, sync);
    syncfuncs.push_back(syncIdent);
}

Synchronizer::Synchronizer(std::string syncIdent): Component(Component::SYNCHRONIZER) {
    syncfuncs.push_back(syncIdent);
}

void Synchronizer::writeTo(RakNet::BitStream& out) {
    unsigned int numIdents = syncfuncs.size();
    out.Write(numIdents);
    std::list<std::string>::iterator ident = syncfuncs.begin();
    for (; ident != syncfuncs.end(); ident++) {
        RakNet::RakString rs(ident->c_str());
        out.Write(rs);
    }
}

void Synchronizer::readFrom(RakNet::BitStream& in) {
    unsigned int numIdents;
    in.Read(numIdents);
    for (unsigned int i = 0; i < numIdents; i++) {
        RakNet::RakString rs;
        in.Read(rs);
        syncfuncs.push_back(rs.C_String());
    }
}

Synchronizer* Synchronizer::get(Object &obj) {
    return (Synchronizer*)obj.getComponent(Component::SYNCHRONIZER);
}

void Synchronizer::add(Object* obj, std::string syncIdent, sync_function sync) {
    Synchronizer *s = (Synchronizer*)obj->getComponent(Component::SYNCHRONIZER);
    if (sync != NULL) {
        Register(syncIdent, sync);
    }
    if (s == NULL) {
        s = new Synchronizer(syncIdent, sync);
        obj->addComponent(s);
    } else {
        s->syncfuncs.push_back(syncIdent);
    }
}

void Synchronizer::Register(std::string syncIdent, sync_function sync) {
    std::map<std::string,sync_function>::iterator func = registeredFunctions.find(syncIdent);
    if (func != registeredFunctions.end())
        printf("Overriding previously registered synchronizer.\n");
    registeredFunctions.insert(std::pair<std::string,sync_function>(syncIdent, sync));
}

void Synchronizer::Execute(std::string syncIdent, Object& obj, RakNet::BitStream &bs, bool write) {
    std::map<std::string,sync_function>::iterator func = registeredFunctions.find(syncIdent);
    if (func == registeredFunctions.end())
        printf("Attempted to execute unregistered synchronizer '%s'\n", syncIdent.c_str());
    else
        (func->second)(obj, bs, write);
}


void Synchronizer::read(RakNet::BitStream &in) {
    for (std::list<std::string>::iterator sync = syncfuncs.begin(); sync != syncfuncs.end(); sync++) {
        Execute(*sync, *this->owner(), in, false);
    }
}

void Synchronizer::write(RakNet::BitStream &out) {
    for (std::list<std::string>::iterator sync = syncfuncs.begin(); sync != syncfuncs.end(); sync++) {
        Execute(*sync, *this->owner(), out, true);
    }
}
