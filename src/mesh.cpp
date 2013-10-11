#include <GL/freeglut.h>

#include <mesh.h>

Mesh::Mesh(): Component(Component::MESH) {

}

//describe a unit plane centered at 0,0,0
void PlaneMesh::describe() {
    glBegin(GL_QUADS);
        glVertex3f(-0.5, 0, 0.5);
        glVertex3f(-0.5, 0, -0.5);
        glVertex3f(0.5, 0, -0.5);
        glVertex3f(0.5, 0, 0.5);
    glEnd();
}
