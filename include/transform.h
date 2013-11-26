#ifndef INCLUDED_TRANSFORM_H
#define INCLUDED_TRANSFORM_H

#include <component.h>
#include <vec.h>

class Transform: public Component {
private:

public:
    vec3f position, rotation, scale;

    Transform(vec3f position=vec3f(0), vec3f rotation=vec3f(0), vec3f scale=vec3f(1));

    void writeTo(RakNet::BitStream& out);
    static void translateObj(Object *obj, const vec3f &delta);
    void readFrom(RakNet::BitStream& in);
    static void rotateObj(Object *obj, const vec3f &delta);

    static void scaleObj(Object *obj, const vec3f &delta);

    static void setObjPosition(Object *obj, const vec3f &pos);
    static void setObjRotation(Object *obj, const vec3f &rot);
    static void setObjScale(Object *obj, const vec3f &scale);

    static Transform* get(Object &obj);
};

#endif
