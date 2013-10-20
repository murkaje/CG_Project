#include "camera.h"
#include "transform.h"
#include "vector3f.h"

#include <GL/freeglut.h>

Camera::Camera(bool perspective): Component(Component::CAMERA) {
    this->perspective = perspective;
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
        if (perspective) gluPerspective(fov, aspect, zNear, zFar);
        else glOrtho(-vSize/2,vSize/2, -vSize/2, vSize/2, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}

