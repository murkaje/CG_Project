#ifndef PHYSICSSUBSYSTEM_H
#define PHYSICSSUBSYSTEM_H

#include <object.h>
#include <collider.h>

class PhysicsSubsystem
{
public:
    static void PerformPhysicsChecks();

    static void checkIntersections(Object &obj);

    static void BoxToBoxIntersection(BoxCollider &obj, BoxCollider &iterObj);
};

#endif
