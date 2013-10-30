#ifndef PHYSICSSUBSYSTEM_H
#define PHYSICSSUBSYSTEM_H

#include <object.h>

class PhysicsSubsystem {
public:
    static void PerformPhysicsChecks();

    static void checkIntersections(Object &obj);

    static void checkBoxToBoxIntersection(Object &obj, Object &iterObj);
};

#endif
