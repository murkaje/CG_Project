#include <object.h>
#include <transform.h>

Transform::Transform(vec3f position, vec3f rotation, vec3f scale):
Component(Component::TRANSFORM), position(position), rotation(rotation), scale(scale) {

}

Transform* Transform::get(Object &obj) {
    return ((Transform*)obj.getComponent(Component::TRANSFORM));
}

void Transform::translateObj(Object *obj, const vec3f &delta) {
    Transform *t = (Transform*)obj->getComponent(Component::TRANSFORM);
    if (t != NULL) {
        t->position += delta;
    }
}

void Transform::rotateObj(Object *obj, const vec3f &delta) {
    Transform *t = (Transform*)obj->getComponent(Component::TRANSFORM);
    if (t != NULL) {
        t->rotation += delta;
    }
}

void Transform::scaleObj(Object *obj, const vec3f &delta) {
    Transform *t = (Transform*)obj->getComponent(Component::TRANSFORM);
    if (t != NULL) {
        t->scale += delta;
    }
}

void Transform::setObjPosition(Object *obj, const vec3f &pos) {
    Transform *t = (Transform*)obj->getComponent(Component::TRANSFORM);
    if (t == NULL) {
        t = new Transform();
        obj->addComponent(t);
    }
    t->position = pos;
}

void Transform::setObjRotation(Object *obj, const vec3f &rot) {
    Transform *t = (Transform*)obj->getComponent(Component::TRANSFORM);
    if (t == NULL) {
        t = new Transform();
        obj->addComponent(t);
    }
    t->rotation = rot;
}

void Transform::setObjScale(Object *obj, const vec3f &scale) {
    Transform *t = (Transform*)obj->getComponent(Component::TRANSFORM);
    if (t == NULL) {
        t = new Transform();
        obj->addComponent(t);
    }
    t->scale = scale;
}
