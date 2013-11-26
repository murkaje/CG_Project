
#include <renderer.h>
#include <mesh.h>
#include <object.h>
#include <material.h>

Renderer::Renderer(int type): Component(Component::RENDERER) {
    this->type = type;
}

void Renderer::writeTo(RakNet::BitStream& out) {
    out.Write(material.shininess);
    out.Write(material.lighting_enabled);
    out << material.diffuse << material.ambient << material.specular;
}

void Renderer::readFrom(RakNet::BitStream& in) {
    in.Read(material.shininess);
    in.Read(material.lighting_enabled);
    in >> material.diffuse >> material.ambient >> material.specular;
}

MeshRenderer::MeshRenderer(vec3f color): Renderer(Renderer::MESH) {
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

        GLint light0_location = glGetUniformLocation(material.shader.prog, "light0");
        int enabled = glIsEnabled(GL_LIGHT0);
        glUniform1f(light0_location, enabled);
        GLint light1_location = glGetUniformLocation (material.shader.prog, "light1");
        enabled = glIsEnabled(GL_LIGHT1);
        glUniform1f(light1_location, enabled);
        GLint light2_location = glGetUniformLocation(material.shader.prog, "light2");
        enabled = glIsEnabled(GL_LIGHT2);
        glUniform1f(light2_location, enabled);
        GLint light3_location = glGetUniformLocation(material.shader.prog, "light3");
        enabled = glIsEnabled(GL_LIGHT3);
        glUniform1f(light3_location, enabled);
        GLint light4_location = glGetUniformLocation(material.shader.prog, "light4");
        enabled = glIsEnabled(GL_LIGHT4);
        glUniform1f(light4_location, enabled);
        GLint light5_location = glGetUniformLocation(material.shader.prog, "light5");
        enabled = glIsEnabled(GL_LIGHT5);
        glUniform1f(light5_location, enabled);
        GLint light6_location = glGetUniformLocation(material.shader.prog, "light6");
        enabled = glIsEnabled(GL_LIGHT6);
        glUniform1f(light6_location, enabled);
        GLint light7_location = glGetUniformLocation(material.shader.prog, "light7");
        enabled = glIsEnabled(GL_LIGHT7);
        glUniform1f(light7_location, enabled);

        variable_location = glGetUniformLocation(material.shader.prog, "lighting_enabled");
        glUniform1i(variable_location, material.lighting_enabled);
        m->describe();
        glUseProgram(0);
    }
}
