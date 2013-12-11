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
    GLfloat n[6][3] =
    {
        {-1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, -1.0}
    };
    GLint faces[6][4] =
    {
        {0, 1, 2, 3},
        {3, 2, 6, 7},
        {7, 6, 5, 4},
        {4, 5, 1, 0},
        {5, 6, 2, 1},
        {7, 4, 0, 3}
    };
    GLfloat v[8][3];
    GLint i;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -0.5;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = 0.5;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -0.5;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = 0.5;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -0.5;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = 0.5;

    for (i = 5; i >= 0; i--) {
        glBegin(GL_QUADS);
            glNormal3fv(&n[i][0]);
            glTexCoord2f(0.0, -1.0);
            glVertex3fv(&v[faces[i][0]][0]);
            glTexCoord2f(-1.0, -1.0);
            glVertex3fv(&v[faces[i][1]][0]);
            glTexCoord2f(-1.0, 0.0);
            glVertex3fv(&v[faces[i][2]][0]);
            glTexCoord2f(0.0, 0.0);
            glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
    }
}
