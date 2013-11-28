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

    GLuint textureHandle;

public:
    GLuint texId; //temporary

    //TODO material cache, so objects can share materials
    Shader &shader;

    static const GLuint NO_TEXTURE = 666;

    vec3f ambient, diffuse, specular;
    float shininess;

    void setTexture(GLuint texId, std::string filename);

    const GLuint getTexture();

    bool lighting_enabled;

    Material(std::string name="default");

    ~Material();

    void describe();

    friend class GraphicsSubsystem;
};

#endif
