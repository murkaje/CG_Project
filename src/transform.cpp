#include <object.h>
#include <transform.h>

Transform::Transform(vector3f position, vector3f rotation, vector3f scale):
Component(Component::TRANSFORM), position(position), rotation(rotation), scale(scale) {

}

Transform* Transform::get(Object &obj) {
    return ((Transform*)obj.getComponent(Component::TRANSFORM));
}

void Transform::translateObj(Object *obj, float dx, float dy, float dz) {
    Transform *t = (Transform*)obj->getComponent(Component::TRANSFORM);
    if (t != NULL) {
        t->position.x += dx;
        t->position.y += dy;
        t->position.z += dz;
    }
}

void Transform::rotateObj(Object *obj, float dx, float dy, float dz) {
    Transform *t = (Transform*)obj->getComponent(Component::TRANSFORM);
    if (t != NULL) {
        t->rotation.x += dx;
        t->rotation.y += dy;
        t->rotation.z += dz;
    }
}

void Transform::scaleObj(Object *obj, float dx, float dy, float dz) {
    Transform *t = (Transform*)obj->getComponent(Component::TRANSFORM);
    if (t != NULL) {
        t->scale.x += dx;
        t->scale.y += dy;
        t->scale.z += dz;
    }
}

void Transform::setObjPosition(Object *obj, float x, float y, float z) {
    Transform *t = (Transform*)obj->getComponent(Component::TRANSFORM);
    if (t == NULL) {
        t = new Transform();
        obj->addComponent(t);
    }
    t->position.x = x;
    t->position.y = y;
    t->position.z = z;
}

void Transform::setObjRotation(Object *obj, float x, float y, float z) {
    Transform *t = (Transform*)obj->getComponent(Component::TRANSFORM);
    if (t == NULL) {
        t = new Transform();
        obj->addComponent(t);
    }
    t->rotation.x = x;
    t->rotation.y = y;
    t->rotation.z = z;
}

void Transform::setObjScale(Object *obj, float x, float y, float z) {
    Transform *t = (Transform*)obj->getComponent(Component::TRANSFORM);
    if (t == NULL) {
        t = new Transform();
        obj->addComponent(t);
    }
    t->scale.x = x;
    t->scale.y = y;
    t->scale.z = z;
}
