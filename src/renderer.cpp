#include <GL/freeglut.h>

#include <renderer.h>
#include <mesh.h>
#include <object.h>

Renderer::Renderer(): Component(Component::RENDERER) {

}

MeshRenderer::MeshRenderer(vector3f color): color(color) {

}

void MeshRenderer::render() {
    Mesh* m = (Mesh*)owner_->getComponent(Component::MESH);
    if (m != NULL) {
        glColor3f(color.x, color.y, color.z);
        m->describe();
    }
}
