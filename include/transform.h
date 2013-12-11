#ifndef INCLUDED_TRANSFORM_H
#define INCLUDED_TRANSFORM_H

#include <component.h>
#include <vec.h>
#include <glm/glm.hpp>

class Transform: public Component {
private:

public:
    glm::vec3 position, rotation, scale;

    Transform(glm::vec3 position=glm::vec3(0), glm::vec3 rotation=glm::vec3(0), glm::vec3 scale=glm::vec3(1));

    void writeTo(RakNet::BitStream& out);
    static void translateObj(Object *obj, const glm::vec3 &delta);
    void readFrom(RakNet::BitStream& in);
    static void rotateObj(Object *obj, const glm::vec3 &delta);

    static void scaleObj(Object *obj, const glm::vec3 &delta);

    static void setObjPosition(Object *obj, const glm::vec3 &pos);
    static void setObjRotation(Object *obj, const glm::vec3 &rot);
    static void setObjScale(Object *obj, const glm::vec3 &scale);

    static Transform* get(Object &obj);
};

#endif
