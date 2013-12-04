#ifndef PHYSICSSUBSYSTEM_H
#define PHYSICSSUBSYSTEM_H

#include <object.h>
#include <collider.h>
#include <glm/glm.hpp>

class PhysicsSubsystem {
public:
    static void PerformPhysicsChecks();

    static void checkIntersections(Object &obj);

    static void BoxToBoxIntersection(BoxCollider &obj, BoxCollider &iterObj);

    static bool RayToBoxIntersection(const glm::vec3 &origin, const glm::vec3 &dir, BoxCollider &other);
    static bool RayToAABBIntersection(const glm::vec3 &origin, const glm::vec3 &dir);
    static bool RayToSphereIntersection(const glm::vec3 &origin, const glm::vec3 &dir, BoxCollider &other);
};

#endif
