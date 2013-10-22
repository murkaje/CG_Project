#include "behavior.h"
#include "light.h"
#include "transform.h"
#include <cstdio>

Light::LightsCache Light::lightsCache;

void Light::update(Object &lightObj) {
    Light *light = (Light*)lightObj.getComponent(Component::LIGHT);
    if (glIsEnabled(light->light) == GL_TRUE) {
        Transform &t = *(Transform*)lightObj.getComponent(Component::TRANSFORM);
        //TODO proper implementation of vector4f
        float ambient[] = {light->ambient.x, light->ambient.y, light->ambient.z, 1.0};
        glLightfv(light->light, GL_AMBIENT, ambient);
        float diffuse[] = {light->diffuse.x, light->diffuse.y, light->diffuse.z, 1.0};
        glLightfv(light->light, GL_DIFFUSE, diffuse);
        float specular[] = {light->specular.x, light->specular.y, light->specular.z, 1.0};
        glLightfv(light->light, GL_SPECULAR, specular);
        float pos[] = {t.position.x, t.position.y, t.position.z, 1.0};
        glLightfv(light->light, GL_POSITION, pos);
        glLightf(light->light, GL_CONSTANT_ATTENUATION, light->constant_attenuation);
        glLightf(light->light, GL_LINEAR_ATTENUATION, light->linear_attenuation);
        glLightf(light->light, GL_QUADRATIC_ATTENUATION, light->quadratic_attenuation);

        float dir[] = {light->direction.x, light->direction.y, light->direction.z};
        glLightfv(light->light, GL_SPOT_DIRECTION, dir);
        glLightf(light->light, GL_SPOT_CUTOFF, light->cutoff);
        glLightf(light->light, GL_SPOT_EXPONENT, light->exponent);
    }
}

Light::Light(bool enabled): Component(Component::LIGHT), ambient(vector3f::zero), diffuse(vector3f::unit), specular(vector3f::unit), direction(vector3f(0,0,-1)) {
    light = Light::lightsCache.getAvailableLight();
    this->enabled(enabled);
    constant_attenuation = 1.0;
    linear_attenuation = 0.0;
    quadratic_attenuation = 0.0;
    exponent = 0.0;
    cutoff = 180.0;
}

void Light::enabled(bool enabled) {
    if (enabled) {
        glEnable(light);
    } else {
        glDisable(light);
    }
}


GLenum Light::LightsCache::getAvailableLight() {
    if (available.empty()) {
        return GL_NONE;
    } else {
        GLenum top = available.top();
        available.pop();
        return top;
    }
}

void Light::LightsCache::returnLight(GLenum light) {
    available.push(light);
}

Object* Light::createPointLight(vector3f position) {
    Object *light = new Object("pointLight");
    Light *l = new Light();
    l->constant_attenuation = 0.0;
    l->linear_attenuation = 0.1;
    l->quadratic_attenuation = 0.01;
    light->addComponent(l);
    light->addComponent(new Transform(position));
    light->addComponent(new Behavior(Light::update));
    return light;
}
