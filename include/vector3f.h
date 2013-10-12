#ifndef INCLUDED_VECTOR3F_H
#define INCLUDED_VECTOR3F_H

struct vector3f {
public:
    static const vector3f zero, unit;

    float x, y, z;
    vector3f (float x, float y): x(x), y(y), z(0) {};
    vector3f (float x, float y, float z): x(x), y(y), z(z) {};

    vector3f operator+ (vector3f v) {
        return vector3f (x + v.x, y + v.y, z + v.z);
    }
    vector3f operator+= (vector3f v) {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }
    vector3f operator-= (vector3f v) {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }
    vector3f operator- (vector3f v) {
        return vector3f (x - v.x, y - v.y, z - v.z);
    }
    vector3f operator* (float f) {
        return vector3f (x * f, y * f, z * f);
    }
    vector3f operator/ (float f) {
        return vector3f (x / f, y / f, z / f);
    }
};

#endif
