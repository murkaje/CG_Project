#ifndef INCLUDED_TRANSFORM_H
#define INCLUDED_TRANSFORM_H

#include <component.h>
#include <vector3f.h>

class Transform: public Component {
private:

public:
    vector3f position, rotation, scale;

    Transform(vector3f position=vector3f::zero, vector3f rotation=vector3f::zero, vector3f scale=vector3f::unit);

    void writeTo(RakNet::BitStream& out);
    void readFrom(RakNet::BitStream& in);

    static void translateObj(Object *obj, float dx, float dy, float dz);
    static void rotateObj(Object *obj, float dx, float dy, float dz);
    static void scaleObj(Object *obj, float dx, float dy, float dz);

    static void setObjPosition(Object *obj, float dx, float dy, float dz);
    static void setObjRotation(Object *obj, float dx, float dy, float dz);
    static void setObjScale(Object *obj, float dx, float dy, float dz);

    static Transform* get(Object &obj);
};

#endif
