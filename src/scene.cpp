#include <GL/freeglut.h>

#include <scene.h>
#include <renderer.h>
#include <transform.h>
#include <behavior.h>

void Scene::init() {

}


void Scene::addObject(Object* object) {
    objects.push_back(*object);
}

void Scene::update() {
    for (std::list<Object>::iterator obj = objects.begin(); obj != objects.end(); obj++) {
        Behavior* b = (Behavior*)obj->getComponent(Component::BEHAVIOR);
        for (std::list<behavior_function>::iterator b1 = b->actions.begin(); b1 != b->actions.end(); b1++) {
            (*b1)(*obj);
        }
    }
}

void Scene::draw() {

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
