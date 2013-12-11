#include "material.h"
#include "graphicssubsystem.h"

#include <stdexcept>
#include <cerrno>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

GLuint compile(GLuint type, std::string source) {
    GLuint shader = glCreateShader(type);
    const GLchar* code[] = { source.c_str() };
    glShaderSource(shader, 1, code, NULL);
    glCompileShader(shader);
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::string log(length, ' ');
        glGetShaderInfoLog(shader, length, &length, &log[0]);
        std::cout << "Shader compilation error: " << log << std::endl;
        throw std::logic_error(log);
        return false;
    }
    return shader;
}

Material::Shader::Shader(std::string name): name(name) {
    if (GraphicsSubsystem::isInit) {
        std::string vertex_shader_filename = std::string("../shaders/") + name + "_vertex.glsl";
        std::string fragment_shader_filename = std::string("../shaders/") + name + "_fragment.glsl";
        v_source = Utils::load(vertex_shader_filename.c_str());
        f_source = Utils::load(fragment_shader_filename.c_str());
        vertex_shader = compile(GL_VERTEX_SHADER, v_source);
        fragment_shader = compile(GL_FRAGMENT_SHADER, f_source);
        prog = glCreateProgram();
        glAttachShader(prog, vertex_shader);
        glAttachShader(prog, fragment_shader);
        glLinkProgram(prog);
        printf("%s\n",("Created shader: "+name).c_str());
    }
}

Material::Shader::~Shader() {
    glDeleteProgram(prog);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    printf("%s\n", ("Deleted shader: "+name).c_str());
}

std::string Material::Texture::getFilename() {
    return filename;
}

GLuint Material::Texture::getId() {
    return texId;
}


GLuint Material::Texture::getHandle() {
    return textureHandle;
}

//texture cache should assign texture IDs
Material::Texture::Texture(GLuint texId, std::string filename): filename(filename) {
    if (filename.length() == 0) {
        this->texId = Texture::EMPTY;
    } else {
        this->texId = texId;
        if (GraphicsSubsystem::isInit) {
            glGenTextures(1, &textureHandle);
            glActiveTexture(GL_TEXTURE0+texId);
            glBindTexture(GL_TEXTURE_2D, textureHandle);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            //glEnable(GL_TEXTURE_GEN_S);
            //glEnable(GL_TEXTURE_GEN_T);
            //glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
            //glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

            Utils::loadTexture(textureHandle, filename.c_str());
            printf("Loaded texture '%s' with unit id %d\n", filename.c_str(), this->texId);
        }
    }
}

Material::Texture::~Texture() {
    if (texId != EMPTY) {
        glDeleteTextures(1, &textureHandle);
        printf("Deleted texture '%s' with unit id %d\n", filename.c_str(), this->texId);
    }
}

Material::Material(std::string name): texture(&GraphicsSubsystem::loadTexture("empty")), shader(&GraphicsSubsystem::loadShader(name)),
    ambient(glm::vec3(0,0,0)), diffuse(glm::vec3(1,1,1)), specular(glm::vec3(0,0,0)) {
    shininess = 1;

    lighting_enabled = true;
}

Material::~Material() {

}

void Material::setTexture(std::string filename) {
    texture = &GraphicsSubsystem::loadTexture(filename);
}

void Material::setShader(std::string name) {
    shader = &GraphicsSubsystem::loadShader(name);
}

void Material::describe() {
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(specular));
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(ambient));
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(diffuse));
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}
