#include "collider.h"

Collider::Collider(int type): Component(Component::COLLIDER) {
    this->type = type;
}

Collider::Collision::Collision(Collider &with, v3f point, v3f normal): with(with), point(point), normal(normal) {

}

void BoxCollider::writeTo(RakNet::BitStream& out) {
    out.WriteVector(center.x, center.y, center.z);
    out.WriteVector(rotation.x, rotation.y, rotation.z);
    out.WriteVector(scale.x, scale.y, scale.z);
}

void BoxCollider::readFrom(RakNet::BitStream& in) {
    in.ReadVector(center.x, center.y, center.z);
    in.ReadVector(rotation.x, rotation.y, rotation.z);
    in.ReadVector(scale.x, scale.y, scale.z);
}

std::list<Collider::Collision>& Collider::collisions() {
    return _collisions;
}

template<> Component* Component::allocate_t<Collider>(int type) {
    Component *newComp = NULL;
    if (type == Collider::BOX) newComp = new BoxCollider();
    return newComp;
};

Collider* Collider::get(Object &obj) {
    return (Collider*)obj.getComponent(Component::COLLIDER);
}

void Collider::addBox(Object &obj) {
    Collider *c = Collider::get(obj);
    if (c != NULL) {
        delete c;
    }
    BoxCollider *bc = new BoxCollider();
    Transform *t = Transform::get(obj);
    if (t != NULL) {
        bc->rotation = t->rotation;
        bc->scale = t->scale;
    }
    obj.addComponent(bc);
}

BoxCollider::BoxCollider(): Collider(Collider::BOX), center(v3f::zero), rotation(v3f::zero), scale(v3f::unit) {

}
