
#include <renderer.h>
#include <mesh.h>
#include <object.h>
#include <material.h>

Renderer::Renderer(): Component(Component::RENDERER) {

}

MeshRenderer::MeshRenderer(vector3f color) {
    material.diffuse = color;
}

Renderer* Renderer::get(Object &obj) {
    return ((Renderer*)obj.getComponent(Component::RENDERER));
}


void MeshRenderer::render() {
    Mesh* m = (Mesh*)owner_->getComponent(Component::MESH);
    if (m != NULL) {
        material.describe();
        glUseProgram(material.shader.prog);
        GLint variable_location = glGetUniformLocation(material.shader.prog, "time");
        glUniform1f(variable_location, Utils::time());
        variable_location = glGetUniformLocation(material.shader.prog, "lighting_enabled");
        glUniform1i(variable_location, material.lighting_enabled);
        m->describe();
        glUseProgram(0);
    }
}
