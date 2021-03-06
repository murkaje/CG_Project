#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <glm/glm.hpp>

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
//TODO material cache
    class Texture {
    private:
        GLuint textureHandle;
        GLuint texId;

        std::string filename;

    public:
        static const GLuint EMPTY = 666;

        std::string getFilename();

        GLuint getId();

        GLuint getHandle();

        Texture(GLuint texId, std::string filename="");

        ~Texture();
    };

    Texture *texture;

    Shader *shader;

    glm::vec3 ambient, diffuse, specular;
    float shininess;

    void setTexture(std::string filename);

    void setShader(std::string name);

    bool lighting_enabled;

    Material(std::string name="default");

    ~Material();

    void describe();

    friend class GraphicsSubsystem;
};

#endif
