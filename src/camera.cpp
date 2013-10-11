#include "camera.h"
#include "transform.h"

Camera::Camera() {
    addComponent(new Transform());

}

