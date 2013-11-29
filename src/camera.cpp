#include "camera.h"
#include "transform.h"
#include "vec.h"

#include <utils.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(bool perspective): Component(Component::CAMERA) {
    this->perspective = perspective;
}


Camera* Camera::get(Object &obj) {
    return (Camera*)obj.getComponent(Component::CAMERA);
}

Object* Camera::createPerspectiveCamera(double fov, double aspect, double zNear, double zFar) {
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
    if(perspective) {
        projMat = glm::perspective(fov, aspect, zNear, zFar);
    } else {
        projMat = glm::ortho(-vSize/2,vSize/2, -vSize/2, vSize/2, zNear, zFar);
    }
    glMatrixMode(GL_PROJECTION);
    /*
        if (perspective) gluPerspective(fov, aspect, zNear, zFar);
        else glOrtho(-vSize/2,vSize/2, -vSize/2, vSize/2, zNear, zFar);
    */
        glLoadMatrixf(glm::value_ptr(projMat));
    glMatrixMode(GL_MODELVIEW);
}

