#include "collider.h"

Collider::Collider(int type): Component(Component::COLLIDER) {
    this->type = type;
}

Collider::Collision::Collision(Collider &with, vec3f point, vec3f normal): with(with), point(point), normal(normal) {

}

void BoxCollider::writeTo(RakNet::BitStream& out) {
    out << center << rotation << scale;
}

void BoxCollider::readFrom(RakNet::BitStream& in) {
    in >> center >> rotation >> scale;
}

std::list<Collider::Collision>& Collider::collisions() {
    return _collisions;
}

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

BoxCollider::BoxCollider(): Collider(Collider::BOX), center(vec3f(0)), rotation(vec3f(0)), scale(vec3f(1)) {

}
