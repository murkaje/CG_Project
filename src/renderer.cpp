
#include <renderer.h>
#include <mesh.h>
#include <object.h>
#include <material.h>

Renderer::Renderer(int type): Component(Component::RENDERER), type(type) {

}

void Renderer::writeTo(RakNet::BitStream& out) {
    out.Write(material.shininess);
    out.Write(material.lighting_enabled);
    out.WriteVector(material.diffuse.x, material.diffuse.y, material.diffuse.z);
    out.WriteVector(material.ambient.x, material.ambient.y, material.ambient.z);
    out.WriteVector(material.specular.x, material.specular.y, material.specular.z);
}

void Renderer::readFrom(RakNet::BitStream& in) {
    in.Read(material.shininess);
    in.Read(material.lighting_enabled);
    in.ReadVector(material.diffuse.x, material.diffuse.y, material.diffuse.z);
    in.ReadVector(material.ambient.x, material.ambient.y, material.ambient.z);
    in.ReadVector(material.specular.x, material.specular.y, material.specular.z);
}

MeshRenderer::MeshRenderer(vector3f color): Renderer(Renderer::MESH) {
    material.diffuse = color;
}

Renderer* Renderer::get(Object &obj) {
    return ((Renderer*)obj.getComponent(Component::RENDERER));
}

template<> Component* Component::allocate_t<Renderer>(int type) {
    Component *newComp = NULL;
    if (type == Renderer::MESH) newComp = new MeshRenderer(v3f::unit);
    return newComp;
};

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
