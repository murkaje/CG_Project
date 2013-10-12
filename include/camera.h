#ifndef CAMERA_H
#define CAMERA_H

#include <object.h>

class Camera: public Object {
private:

public:
    Camera();

    virtual void setup() {};
};

class PerspectiveCamera: public Camera {
private:

public:
    int fov;
    double zNear, zFar, aspectRatio;

    PerspectiveCamera(int fov, double aspectRatio, double zNear, double zFar);

    void setup();
};

#endif
