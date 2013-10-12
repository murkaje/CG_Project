#include "camera.h"
#include "transform.h"
#include "vector3f.h"

#include <GL/freeglut.h>

Camera::Camera(double zNear, double zFar): zNear(zNear), zFar(zFar) {
    addComponent(&transform);
}

PerspectiveCamera::PerspectiveCamera(int fov, double aspect, double zNear, double zFar): Camera(zNear, zFar), fov(fov), aspect(aspect) {

}


void PerspectiveCamera::setup() {
    glMatrixMode(GL_PROJECTION);
        gluPerspective(fov, aspect, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}

OrthographicCamera::OrthographicCamera(double vSize, double zNear, double zFar): Camera(zNear, zFar), vSize(vSize) {

}

void OrthographicCamera::setup() {
    glMatrixMode(GL_PROJECTION);
        glOrtho(-vSize/2,vSize/2, -vSize/2, vSize/2, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}
