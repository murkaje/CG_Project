#include <GL/freeglut.h>

#include <mesh.h>

Mesh::Mesh(): Component(Component::MESH) {

}

//describe a unit plane centered at 0,0,0
void PlaneMesh::describe() {
    glBegin(GL_QUADS);
        glNormal3f(0.0, 1.0, 0.0);
        glVertex3f(-0.5, 0, 0.5);
        glVertex3f(-0.5, 0, -0.5);
        glVertex3f(0.5, 0, -0.5);
        glVertex3f(0.5, 0, 0.5);
    glEnd();
}

void SphereMesh::describe() {
    glutSolidSphere(0.5,10,10);
}

void CubeMesh::describe() {
    glutSolidCube(1);
}
