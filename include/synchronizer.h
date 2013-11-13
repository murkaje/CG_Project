#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H

#include <component.h>

#include "BitStream.h"

#include <list>

typedef void (*sync_function)(Object&, RakNet::BitStream&, bool);

class Synchronizer: public Component
{
protected:
    void read(RakNet::BitStream &inBs);
    void write(RakNet::BitStream &outBs);
public:
    std::list<sync_function> syncfuncs;

    Synchronizer(sync_function syncfunc);
    Synchronizer(std::list<sync_function> syncfuncs);

    static void add(Object* obj, sync_function syncfunc);

    static Synchronizer* get(Object &obj);

    friend class NetworkSubsystem;
};

#endif
