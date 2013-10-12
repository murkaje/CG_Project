#include "camera.h"
#include "transform.h"

#include <GL/freeglut.h>

Camera::Camera() {
    addComponent(new Transform());

}

PerspectiveCamera::PerspectiveCamera(int fov, double aspectRatio, double zNear, double zFar):fov(fov), aspectRatio(aspectRatio), zNear(zNear), zFar(zFar) {

}


void PerspectiveCamera::setup() {
    // Set up perspective
    glMatrixMode(GL_PROJECTION);
        gluPerspective(fov, aspectRatio, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}
