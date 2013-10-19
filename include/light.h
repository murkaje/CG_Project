#ifndef LIGHT_H
#define LIGHT_H

#include <object.h>
#include <vector3f.h>

#include <stack>
#include <GL/freeglut.h>

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
protected:
    static void update(Object &lightObj);

    Light(bool enabled=true);
public:
    vector3f ambient, diffuse, specular;
    float exponent, cutoff, constant_attenuation, linear_attenuation, quadratic_attenuation;

    void enabled(bool enabled);

    static Object* createPointLight(vector3f position);
};

#endif
