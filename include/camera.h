#ifndef CAMERA_H
#define CAMERA_H

#include <object.h>
#include <transform.h>
#include <glm/glm.hpp>

class Camera: public Component {
public:
    bool perspective;
    double zNear, zFar;
    double fov;
    double vSize, aspect;

    glm::mat4 projMat;

    void writeTo(RakNet::BitStream& out);
    void readFrom(RakNet::BitStream& in);

    Camera(bool perspective=true);

    void setup();

    static Object* createPerspectiveCamera(double fov, double aspect, double zNear, double zFar);
    static Object* createOrthographicCamera(double vSize, double zNear, double zFar);

    static Camera* get(Object &obj);
};

#endif
