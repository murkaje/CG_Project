#include "behavior.h"
#include "light.h"
#include "transform.h"
#include <cstdio>

Light::LightsCache Light::lightsCache;

void Light::update(Object &lightObj) {
    Light *light = (Light*)lightObj.getComponent(Component::LIGHT);
    if (light->enabled) {
        GLenum l = Light::lightsCache.getAvailableLight();
        if (l != GL_NONE) {
            if (glIsEnabled(l) != GL_TRUE) {
                glEnable(l);
            }
            Transform &t = *(Transform*)lightObj.getComponent(Component::TRANSFORM);
            glLightfv(l, GL_AMBIENT, light->ambient.data());
            glLightfv(l, GL_DIFFUSE, light->diffuse.data());
            glLightfv(l, GL_SPECULAR, light->specular.data());
            glLightfv(l, GL_POSITION, vec4f(t.position, 1.0).data());

            //not currently being used
            //glLightf(l, GL_CONSTANT_ATTENUATION, light->constant_attenuation);
            //glLightf(l, GL_LINEAR_ATTENUATION, light->linear_attenuation);
            //glLightf(l, GL_QUADRATIC_ATTENUATION, light->quadratic_attenuation);

            glLightfv(l, GL_SPOT_DIRECTION, light->direction.data());
            glLightf(l, GL_SPOT_CUTOFF, light->cutoff);
            glLightf(l, GL_SPOT_EXPONENT, light->exponent);
        } else {
            //TODO stop lighting updates from running on the server
            //printf("Could not allocate additional lights (8 used)");
        }
    }
}

void Light::setEnabled(bool enabled) {
    this->enabled = enabled;
}

Light::Light(bool enabled): Component(Component::LIGHT), ambient(vec4f(0)), diffuse(vec4f(1)), specular(vec4f(1)), direction(0,-1,0) {
    setEnabled(enabled);
    constant_attenuation = 1.0;
    linear_attenuation = 0.0;
    quadratic_attenuation = 0.0;
    exponent = 0.0;
    cutoff = 180.0;
}

void Light::writeTo(RakNet::BitStream& out) {
    out.Write(enabled);
    out.Write(exponent);
    out.Write(cutoff);
    out.Write(constant_attenuation);
    out.Write(linear_attenuation);
    out.Write(quadratic_attenuation);
    out << ambient << diffuse << specular << direction;
}

void Light::readFrom(RakNet::BitStream& in) {
    in.Read(enabled);
    in.Read(exponent);
    in.Read(cutoff);
    in.Read(constant_attenuation);
    in.Read(linear_attenuation);
    in.Read(quadratic_attenuation);
    in >> ambient >> diffuse >> specular >> direction;
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

void Light::LightsCache::reset() {
    while (!available.empty())
        available.pop();
    available.push(GL_LIGHT7);glDisable(GL_LIGHT7);
    available.push(GL_LIGHT6);glDisable(GL_LIGHT6);
    available.push(GL_LIGHT5);glDisable(GL_LIGHT5);
    available.push(GL_LIGHT4);glDisable(GL_LIGHT4);
    available.push(GL_LIGHT3);glDisable(GL_LIGHT3);
    available.push(GL_LIGHT2);glDisable(GL_LIGHT2);
    available.push(GL_LIGHT1);glDisable(GL_LIGHT1);
    available.push(GL_LIGHT0);glDisable(GL_LIGHT0);
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
