#ifndef LIGHT_H
#define LIGHT_H

#include <object.h>
#include <vec.h>

#include <stack>

#include <utils.h>

class Light : public Component {
private:
    struct LightsCache {
        std::stack<GLenum> available;

        GLenum getAvailableLight();
        void returnLight(GLenum light);

        LightsCache() {
            available.push(GL_LIGHT7);
            available.push(GL_LIGHT6);
            available.push(GL_LIGHT5);
            available.push(GL_LIGHT4);
            available.push(GL_LIGHT3);
            available.push(GL_LIGHT2);
            available.push(GL_LIGHT1);
            available.push(GL_LIGHT0);
        }
    };
    static LightsCache lightsCache;

    GLenum light;

    static void update(Object &lightObj);

    Light(bool enabled=true);
public:
    vec4f ambient, diffuse, specular;
    vec3f direction;
    float exponent, cutoff, constant_attenuation, linear_attenuation, quadratic_attenuation;

    void enabled(bool enabled);

    void writeTo(RakNet::BitStream& out);
    static Object* createPointLight(vec3f position);
    void readFrom(RakNet::BitStream& in);

    friend class GraphicsSubsystem;
    friend class Component;
};

#endif
