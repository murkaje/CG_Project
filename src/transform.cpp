#include <object.h>
#include <transform.h>

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale):
Component(Component::TRANSFORM), position(position), rotation(rotation), scale(scale) {

}

Transform* Transform::get(Object &obj) {
    return ((Transform*)obj.getComponent(Component::TRANSFORM));
}

void Transform::writeTo(RakNet::BitStream& out) {
    out << position << rotation << scale;
}

void Transform::readFrom(RakNet::BitStream& in) {
    in >> position >> rotation >> scale;
}

void Transform::translateObj(Object *obj, const glm::vec3 &delta) {
    Transform *t = (Transform*)obj->getComponent(Component::TRANSFORM);
    if (t != NULL) {
        t->position += delta;
    }
}

void Transform::rotateObj(Object *obj, const glm::vec3 &delta) {
    Transform *t = (Transform*)obj->getComponent(Component::TRANSFORM);
    if (t != NULL) {
        t->rotation += delta;
    }
}

void Transform::scaleObj(Object *obj, const glm::vec3 &delta) {
    Transform *t = (Transform*)obj->getComponent(Component::TRANSFORM);
    if (t != NULL) {
        t->scale += delta;
    }
}

void Transform::setObjPosition(Object *obj, const glm::vec3 &pos) {
    Transform *t = (Transform*)obj->getComponent(Component::TRANSFORM);
    if (t == NULL) {
        t = new Transform();
        obj->addComponent(t);
    }
    t->position = pos;
}

void Transform::setObjRotation(Object *obj, const glm::vec3 &rot) {
    Transform *t = (Transform*)obj->getComponent(Component::TRANSFORM);
    if (t == NULL) {
        t = new Transform();
        obj->addComponent(t);
    }
    t->rotation = rot;
}

void Transform::setObjScale(Object *obj, const glm::vec3 &scale) {
    Transform *t = (Transform*)obj->getComponent(Component::TRANSFORM);
    if (t == NULL) {
        t = new Transform();
        obj->addComponent(t);
    }
    t->scale = scale;
}
