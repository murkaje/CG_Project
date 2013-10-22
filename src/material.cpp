#include "material.h"

#include <stdexcept>
#include <cerrno>
#include <iostream>

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
}

Material::Shader::~Shader() {
    glDeleteProgram(prog);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

Material::Material(std::string name): shader(new Shader(name)), ambient(v3f::zero), diffuse(v3f::unit), specular(v3f::zero) {
    shininess = 1;
}

void Material::describe() {
    //still need to do that proper vector3f implementation
    float spec[] = {specular.x, specular.y, specular.z, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
    float amb[] = {ambient.x, ambient.y, ambient.z, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
    float diff[] = {diffuse.x, diffuse.y, diffuse.z, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

Material::~Material() {
    delete shader;
}
