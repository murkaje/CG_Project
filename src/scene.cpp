#include <GL/freeglut.h>

#include <scene.h>
#include <renderer.h>
#include <transform.h>
#include <behavior.h>
#include <camera.h>

void Scene::init() {
    if (camera != NULL) {
        camera->setup();
    }
}


void Scene::setCamera(Camera *camera) {
    if (this->camera != NULL) {
        delete this->camera;
    }
    this->camera = camera;
}

void Scene::addObject(Object* object) {
    objects.push_back(*object);
}

void Scene::update() {
    Behavior* cb = (Behavior*)camera->getComponent(Component::BEHAVIOR);
    if (cb != NULL) {
        for (std::list<behavior_function>::iterator b1 = cb->actions.begin(); b1 != cb->actions.end(); b1++) {
            (*b1)(*camera);
        }
    }
    for (std::list<Object>::iterator obj = objects.begin(); obj != objects.end(); obj++) {
        Behavior* b = (Behavior*)obj->getComponent(Component::BEHAVIOR);
        if (b != NULL) {
            for (std::list<behavior_function>::iterator b1 = b->actions.begin(); b1 != b->actions.end(); b1++) {
                (*b1)(*obj);
            }
        }
    }
}

void Scene::draw() {
    glLoadIdentity();
    Transform *ct = (Transform*)camera->getComponent(Component::TRANSFORM);
    glRotatef(ct->rotation.x,1.0,0.0,0.0);
    glRotatef(ct->rotation.y,0.0,1.0,0.0);
    glRotatef(ct->rotation.z,0.0,0.0,1.0);
    glScalef(ct->scale.x,ct->scale.y,ct->scale.z);
    gluLookAt(ct->position.x,ct->position.y,ct->position.z,
            0, 0, 0,
            0, 1, 0);

    for (std::list<Object>::iterator obj = objects.begin(); obj != objects.end(); obj++) {
        Renderer* r = (Renderer*)obj->getComponent(Component::RENDERER);
        if (r != NULL) {
            glPushMatrix();
            Transform* t = (Transform*)obj->getComponent(Component::TRANSFORM);
            if (t == NULL) {
                continue;
            } else {
                glTranslatef(t->position.x,t->position.y,t->position.z);
                glRotatef(t->rotation.x,1.0,0.0,0.0);
                glRotatef(t->rotation.y,0.0,1.0,0.0);
                glRotatef(t->rotation.z,0.0,0.0,1.0);
                glScalef(t->scale.x,t->scale.y,t->scale.z);
            }
            r->render();

            glPopMatrix();
        }
    }

}

Scene::~Scene() {
    delete camera;
}
