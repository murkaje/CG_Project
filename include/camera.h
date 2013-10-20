#ifndef CAMERA_H
#define CAMERA_H

#include <object.h>
#include <transform.h>

class Camera: public Component {
private:

public:
    bool perspective;
    double zNear, zFar;
    int fov;
    double vSize, aspect;

    Camera(bool perspective=true);

    void setup();

    static Object* createPerspectiveCamera(int fov, double aspect, double zNear, double zFar);
    static Object* createOrthographicCamera(double vSize, double zNear, double zFar);
};

#endif
