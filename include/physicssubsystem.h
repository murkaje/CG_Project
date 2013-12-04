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

    static float RayToBoxIntersection(const glm::vec3 &origin, const glm::vec3 &dir, BoxCollider &other);
    static glm::vec3 RayToAABBIntersection(const glm::vec3 &origin, const glm::vec3 &dir);
    static float RayToSphereIntersection(const glm::vec3 &origin, const glm::vec3 &dir, BoxCollider &other);
};

#endif
