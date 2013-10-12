#ifndef INCLUDED_TRANSFORM_H
#define INCLUDED_TRANSFORM_H

#include <component.h>
#include <vector3f.h>

class Transform: public Component {
private:

public:
    vector3f position, rotation, scale;

    Transform(vector3f position=vector3f::zero, vector3f rotation=vector3f::zero, vector3f scale=vector3f::unit);
};

#endif
