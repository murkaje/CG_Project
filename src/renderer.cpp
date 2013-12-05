
#include <renderer.h>
#include <mesh.h>
#include <object.h>
#include <material.h>

bool NO_SHADER = false;

Renderer::Renderer(int type): Component(Component::RENDERER) {
    this->type = type;
    cast_shadows = true;
    receive_shadows = true;
}

void Renderer::writeTo(RakNet::BitStream& out) {
    out.Write(material.shininess);
    out.Write(material.lighting_enabled);
    out.Write(receive_shadows);
    out.Write(cast_shadows);
    RakNet::RakString rs(material.texture->getFilename().c_str());
    out.Write(rs);
    RakNet::RakString rs2(material.shader->name.c_str());
    out.Write(rs2);
    out << material.diffuse << material.ambient << material.specular;
}

void Renderer::readFrom(RakNet::BitStream& in) {
    in.Read(material.shininess);
    in.Read(material.lighting_enabled);
    in.Read(receive_shadows);
    in.Read(cast_shadows);
    RakNet::RakString rs;
    in.Read(rs);
    material.setTexture(rs.C_String());
    RakNet::RakString rs2;
    in.Read(rs2);
    printf("setting shader %s\n", rs2.C_String());
    material.setShader(rs2.C_String());
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
    Mesh* m = Mesh::get(*owner_);
    if (m != NULL) {
        material.describe();
        if (!NO_SHADER) {
            glUseProgram(material.shader->prog);
            GLint variable_location = glGetUniformLocation(material.shader->prog, "time");
            glUniform1f(variable_location, Utils::time());

            //enable/disable whether object is affected by lights
            variable_location = glGetUniformLocation(material.shader->prog, "lighting_enabled");
            glUniform1i(variable_location, material.lighting_enabled);
            if (material.lighting_enabled) {
                //enabled/disable light sources
                GLint lightingEnabled = glIsEnabled(GL_LIGHTING);
                for (int i = 0; i < 8; i++) {
                    toggleLightInShader(material.shader->prog, lightingEnabled, i);
                }
            }

            GLint has_tex = glGetUniformLocation(material.shader->prog, "texId");
            GLuint texId = material.texture->getId();
            if (texId != Material::Texture::EMPTY) {
                glUniform1i(has_tex, texId);
                glActiveTexture(GL_TEXTURE0+texId);
                GLuint texture = material.texture->getHandle();
                glBindTexture(GL_TEXTURE_2D, texture);
                GLint tex = glGetUniformLocation(material.shader->prog, "texture");
                glUniform1i(tex, texture);
                glActiveTexture(0);
            } else {
                glUniform1i(has_tex, Material::Texture::EMPTY);
            }
            variable_location = glGetUniformLocation(material.shader->prog, "receive_shadows");
            glUniform1i(variable_location, receive_shadows);

            m->describe();
            glUseProgram(0);
        } else {
            if (cast_shadows) {
                m->describe();
            }
        }
    }
}
