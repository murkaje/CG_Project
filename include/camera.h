#ifndef CAMERA_H
#define CAMERA_H

#include <object.h>

class Camera: public Object {
private:

public:
    Camera();
};

class PerspectiveCamera: public Camera {
private:

public:
    PerspectiveCamera();
};

#endif
