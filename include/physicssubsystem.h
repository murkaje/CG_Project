#ifndef PHYSICSSUBSYSTEM_H
#define PHYSICSSUBSYSTEM_H

#include<object.h>

class PhysicsSubsystem
{
    public:
        PhysicsSubsystem();
        virtual ~PhysicsSubsystem();
        static void PerformPhysicsChecks();
        static void checkIntersection(Object &obj);
    protected:
    private:
};

#endif
