#include "behavior.h"
#include "light.h"
#include "transform.h"
#include <cstdio>

Light::LightsCache Light::lightsCache;

void Light::update(Object &lightObj) {
    Light *light = (Light*)lightObj.getComponent(Component::LIGHT);
    if (glIsEnabled(light->light) == GL_TRUE) {
        Transform &t = *(Transform*)lightObj.getComponent(Component::TRANSFORM);
        glLightfv(light->light, GL_AMBIENT, light->ambient.data());
        glLightfv(light->light, GL_DIFFUSE, light->diffuse.data());
        glLightfv(light->light, GL_SPECULAR, light->specular.data());
        glLightfv(light->light, GL_POSITION, vec4f(t.position, 1.0).data());

        glLightf(light->light, GL_CONSTANT_ATTENUATION, light->constant_attenuation);
        glLightf(light->light, GL_LINEAR_ATTENUATION, light->linear_attenuation);
        glLightf(light->light, GL_QUADRATIC_ATTENUATION, light->quadratic_attenuation);

        glLightfv(light->light, GL_SPOT_DIRECTION, light->direction.data());
        glLightf(light->light, GL_SPOT_CUTOFF, light->cutoff);
        glLightf(light->light, GL_SPOT_EXPONENT, light->exponent);
    }
}

Light::Light(bool enabled): Component(Component::LIGHT), ambient(vec4f(0)), diffuse(vec4f(1)), specular(vec4f(1)), direction(0,0,-1) {
    light = Light::lightsCache.getAvailableLight();
    this->enabled(enabled);
    constant_attenuation = 1.0;
    linear_attenuation = 0.0;
    quadratic_attenuation = 0.0;
    exponent = 0.0;
    cutoff = 180.0;
}

void Light::writeTo(RakNet::BitStream& out) {
    out.Write(exponent);
    out.Write(cutoff);
    out.Write(constant_attenuation);
    out.Write(linear_attenuation);
    out.Write(quadratic_attenuation);
    out << ambient << diffuse << specular << direction;
}

void Light::readFrom(RakNet::BitStream& in) {
    light = Light::lightsCache.getAvailableLight();
    in.Read(exponent);
    in.Read(cutoff);
    in.Read(constant_attenuation);
    in.Read(linear_attenuation);
    in.Read(quadratic_attenuation);
    in >> ambient >> diffuse >> specular >> direction;
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

Object* Light::createPointLight(vec3f position) {
    Object *light = new Object("pointLight");
    Light *l = new Light();
    l->constant_attenuation = 0.0;
    l->linear_attenuation = 0.0;
    l->quadratic_attenuation = 1.0;
    light->addComponent(l);
    Transform::setObjPosition(light, position);
    Behavior::add(light, "lightUpdate");
    return light;
}
