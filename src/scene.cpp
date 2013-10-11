#include <GL/freeglut.h>

#include <scene.h>
#include <renderer.h>
#include <transform.h>

void Scene::addObject(Object* object) {
    objects.push_back(*object);
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
                glRotatef(t->rotation.x,t->rotation.y,t->rotation.z,1.0);
                glScalef(t->scale.x,t->scale.y,t->scale.z);
            }
            r->render();

            glPopMatrix();
        }
    }

}
