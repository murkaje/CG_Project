#include "synchronizer.h"
#include <object.h>

Synchronizer::Synchronizer(sync_function syncfunc): Component(Component::SYNCHRONIZER) {
    syncfuncs.push_back(syncfunc);
}

Synchronizer::Synchronizer(std::list<sync_function> syncfuncs): Component(Component::SYNCHRONIZER), syncfuncs(syncfuncs) {

}

Synchronizer* Synchronizer::get(Object &obj) {
    return (Synchronizer*)obj.getComponent(Component::SYNCHRONIZER);
}

void Synchronizer::add(Object* obj, sync_function syncfunc) {
    Synchronizer *sync = (Synchronizer*)obj->getComponent(Component::SYNCHRONIZER);
    if (sync == NULL) {
        sync = new Synchronizer(syncfunc);
        obj->addComponent(sync);
    } else {
        sync->syncfuncs.push_back(syncfunc);
    }
}


void Synchronizer::read(RakNet::BitStream &inBs) {
    for (std::list<sync_function>::iterator sync = syncfuncs.begin(); sync != syncfuncs.end(); sync++) {
        (*sync)(*this->owner(), inBs, false);
    }
}

void Synchronizer::write(RakNet::BitStream &outBs) {
    for (std::list<sync_function>::iterator sync = syncfuncs.begin(); sync != syncfuncs.end(); sync++) {
        (*sync)(*this->owner(), outBs, true);
    }
}
