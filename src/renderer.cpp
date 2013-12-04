
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

void toggleLightInShader(GLint prog, GLint lightingEnabled, int i) {
    char lightStr[9];
    sprintf(lightStr, "light[%d]", i);
    GLint light_location = glGetUniformLocation(prog, lightStr);
    int enabled = lightingEnabled && glIsEnabled(GL_LIGHT0+i);
    glUniform1i(light_location, enabled);
}

void MeshRenderer::render() {
    Mesh* m = (Mesh*)owner_->getComponent(Component::MESH);
    if (m != NULL) {
        material.describe();
        glUseProgram(material.shader.prog);
        GLint variable_location = glGetUniformLocation(material.shader.prog, "time");
        glUniform1f(variable_location, Utils::time());

        //enable/disable whether object is affected by lights
        variable_location = glGetUniformLocation(material.shader.prog, "lighting_enabled");
        glUniform1i(variable_location, material.lighting_enabled);
        if (material.lighting_enabled) {
            //enabled/disable light sources
            GLint lightingEnabled = glIsEnabled(GL_LIGHTING);
            for (int i = 0; i < 8; i++) {
                toggleLightInShader(material.shader.prog, lightingEnabled, i);
            }
        }

        GLint has_tex = glGetUniformLocation(material.shader.prog, "texId");
        if (material.texId != Material::NO_TEXTURE) {
            glActiveTexture(GL_TEXTURE0+material.texId);
            glBindTexture(GL_TEXTURE_2D, material.getTexture());
            glUniform1i(has_tex, material.texId);
            GLint tex = glGetUniformLocation(material.shader.prog, "texture");
            glUniform1i(tex, material.getTexture());
        } else {
            glUniform1i(has_tex, Material::NO_TEXTURE);
        }

        m->describe();
        glUseProgram(0);
    }
}
