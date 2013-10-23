#ifndef INCLUDED_BEHAVIOR_H
#define INCLUDED_BEHAVIOR_H

#include <component.h>

#include <list>

typedef void (*behavior_function)(Object&);

class Behavior: public Component {
private:

public:
    std::list<behavior_function> actions;

    Behavior(behavior_function action);
    Behavior(std::list<behavior_function> actions);

    static void add(Object* obj, behavior_function action);
};

#endif
