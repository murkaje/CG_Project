#include <utils.h>
#include <mesh.h>
#include <object.h>

Mesh::Mesh(int type): Component(Component::MESH) {
    this->type = type;
}

void Mesh::writeTo(RakNet::BitStream& out) {

}

void Mesh::readFrom(RakNet::BitStream& in) {

}


Mesh* Mesh::get(Object &obj) {
    return (Mesh*)obj.getComponent(Component::MESH);
}

PlaneMesh::PlaneMesh(): Mesh(Mesh::PLANE) {

}

//describe a unit plane centered at 0,0,0
void PlaneMesh::describe() {
    glFrontFace(GL_CW);
    glBegin(GL_QUADS);
        glNormal3f(0.0, 1.0, 0.0);
        glVertex3f(-0.5, 0, 0.5);
        glVertex3f(-0.5, 0, -0.5);
        glVertex3f(0.5, 0, -0.5);
        glVertex3f(0.5, 0, 0.5);
    glEnd();
    glFrontFace(GL_CCW);
}

SphereMesh::SphereMesh(): Mesh(Mesh::SPHERE) {

}

void SphereMesh::describe() {
    glutSolidSphere(0.5,10,10);
}

CubeMesh::CubeMesh(): Mesh(Mesh::CUBE) {

}

void CubeMesh::describe() {
    glutSolidCube(1);
}
