#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H

#include <component.h>

#include "BitStream.h"

#include <list>
#include <map>
#include <string>

typedef void (*sync_function)(Object&, RakNet::BitStream&, bool);

class Synchronizer: public Component
{
private:
    RakNet::BitStream bs;
    static std::map<std::string, sync_function> registeredFunctions;
protected:
    void read(RakNet::BitStream &in);
    void write(RakNet::BitStream &out);

    Synchronizer();
public:
    std::list<std::string> syncfuncs;

    Synchronizer(std::string actionIdent, sync_function sync);
    Synchronizer(std::string actionIdent);

    void writeTo(RakNet::BitStream& out);
    void readFrom(RakNet::BitStream& in);

    static void add(Object* obj, std::string syncIdent, sync_function sync=NULL);

    static Synchronizer* get(Object &obj);

    static void Register(std::string actionIdent, sync_function sync);

    static void Execute(std::string actionIdent, Object& obj, RakNet::BitStream &bs, bool write);

    friend class NetworkSubsystem;
    friend class Component;
    friend class Object;
};

#endif
