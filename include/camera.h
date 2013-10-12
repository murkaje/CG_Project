#ifndef CAMERA_H
#define CAMERA_H

#include <object.h>
#include <transform.h>

class Camera: public Object {
private:

public:
    double zNear, zFar;

    Camera(double zNear, double zFar);
    Transform transform;

    virtual void setup() {};
};

class PerspectiveCamera: public Camera {
private:

public:
    int fov;
    double aspect;

    PerspectiveCamera(int fov, double aspect, double zNear, double zFar);

    void setup();
};

class OrthographicCamera: public Camera {
private:

public:
    double vSize;

    OrthographicCamera(double vSize, double zNear, double zFar);

    void setup();
};

#endif
