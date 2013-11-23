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

        void reset();

        LightsCache() {
            reset();
        };
    };
    static LightsCache lightsCache;

    bool enabled;

    static void update(Object &lightObj);

    Light::Light(bool enabled = true);
public:
    vec4f ambient, diffuse, specular;
    vec3f direction;
    float exponent, cutoff, constant_attenuation, linear_attenuation, quadratic_attenuation;

    void setEnabled(bool enabled);

    void writeTo(RakNet::BitStream& out);
    static Object* createPointLight(vec3f position);
    void readFrom(RakNet::BitStream& in);

    friend class GraphicsSubsystem;
    friend class Component;
};

#endif
