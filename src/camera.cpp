#include "camera.h"
#include "transform.h"
#include "vec.h"

#include <utils.h>

void Camera::writeTo(RakNet::BitStream& out) {
    out.Write(perspective);
    out.Write(zNear);
    out.Write(zFar);
    out.Write(fov);
    out.Write(vSize);
    out.Write(aspect);
}

void Camera::readFrom(RakNet::BitStream& in) {
    in.Read(perspective);
    in.Read(zNear);
    in.Read(zFar);
    in.Read(fov);
    in.Read(vSize);
    in.Read(aspect);
}

Camera::Camera(bool perspective): Component(Component::CAMERA) {
    this->perspective = perspective;
}

Camera* Camera::get(Object &obj) {
    return (Camera*)obj.getComponent(Component::CAMERA);
}

Object* Camera::createPerspectiveCamera(int fov, double aspect, double zNear, double zFar) {
    Object *camera = new Object("perspectiveCamera");
    Camera *c = new Camera();
    c->fov = fov;
    c->aspect = aspect;
    c->zNear = zNear;
    c->zFar = zFar;
    camera->addComponent(c);
    camera->addComponent(new Transform());
    return camera;
}

Object* Camera::createOrthographicCamera(double vSize, double zNear, double zFar) {
    Object *camera = new Object("orthographicCamera");
    Camera *c = new Camera(false);
    c->vSize = vSize;
    c->zNear = zNear;
    c->zFar = zFar;
    camera->addComponent(c);
    camera->addComponent(new Transform());
    return camera;
}

void Camera::setup() {
    glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (perspective) gluPerspective(fov, aspect, zNear, zFar);
        else glOrtho(-vSize/2,vSize/2, -vSize/2, vSize/2, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}

