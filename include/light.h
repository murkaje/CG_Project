#ifndef LIGHT_H
#define LIGHT_H

#include <object.h>
#include <vector3f.h>

#include <stack>
#include <GL/freeglut.h>

class Light : public Object {
private:
    struct LightsCache {
        std::stack<GLenum> available;

        GLenum getAvailableLight();
        void returnLight(GLenum light);

        LightsCache() {
            available.push(GL_LIGHT0);
        }
    };
    static LightsCache lightsCache;

    GLenum light;
protected:
    static void update(Object &lightObj);
public:
    Transform &transform;
    vector3f ambient, diffuse, specular;
    float exponent, cutoff, constant_attenuation, linear_attenuation, quadratic_attenuation;

    Light(Transform *transform, bool enabled=true);

    void enabled(bool enabled);

    static Light* createPointLight(vector3f position);
};

#endif
