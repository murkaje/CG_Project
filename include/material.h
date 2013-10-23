#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

#include <utils.h>

class Material {
private:
    class Shader {
    private:
        GLuint vertex_shader, fragment_shader;
        std::string v_source, f_source;
    public:
        std::string name;
        GLuint prog;

        Shader(std::string name="default");

        ~Shader();
    };

public:
    //TODO material/shader cache, so objects can share shaders and/or materials
    Shader &shader;

    vector3f ambient, diffuse, specular;
    float shininess;

    Material(std::string name="default");

    void describe();

    friend class GraphicsSubsystem;
};

#endif
