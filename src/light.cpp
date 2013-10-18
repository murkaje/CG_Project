#include "behavior.h"
#include "light.h"
#include "transform.h"
#include <cstdio>

Light::LightsCache Light::lightsCache;


void Light::update(Object &lightObj) {
    Light *light = (Light*)&lightObj;
    if (glIsEnabled(light->light) == GL_TRUE) {
        Transform &t = light->transform;
        float pos[] = {t.position.x, t.position.y, t.position.z, 1};
        glLightfv(light->light, GL_POSITION, pos);
    }
}

Light::Light(Transform *transform, bool enabled): transform(*transform), ambient(vector3f::zero), diffuse(vector3f::zero), specular(vector3f::zero) {
    addComponent(transform);
    light = Light::lightsCache.getAvailableLight();
    addComponent(new Behavior(Light::update));
    this->enabled(enabled);
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

Light* Light::createPointLight(vector3f position) {
    return new Light(new Transform(position));
}
