
#include <utils.h>
#include <scene.h>
#include <renderer.h>
#include <transform.h>
#include <behavior.h>
#include <camera.h>

void positionCamera(Camera* camera) {
    glLoadIdentity();
    if (camera != NULL) {
        Object* parent = camera->owner();
        while (parent != NULL) {
            Transform* t = (Transform*)parent->getComponent(Component::TRANSFORM);
            glRotatef(-t->rotation.z,0.0,0.0,1.0);
            glRotatef(-t->rotation.y,0.0,1.0,0.0);
            glRotatef(-t->rotation.x,1.0,0.0,0.0);
            glTranslatef(-t->position.x,-t->position.y,-t->position.z);
            parent = parent->parent();
        }
    }
}

void Scene::init() {
    Object* cam = Object::Find("MainCamera");
    if (cam != NULL) {
        setCamera((Camera*)cam->getComponent(Component::CAMERA));
        camera->setup();
    }
}


void Scene::setCamera(Camera *camera) {
    this->camera = camera;
}

void Scene::addObject(Object* object) {
    object->registerWithScene(this);
    for (std::list<Object>::iterator obj = object->getChildren().begin(); obj != object->getChildren().end(); obj++) {
        (*obj).registerWithScene(this);
    }
    objects.push_back(*object);
}

void Scene::update() {
    positionCamera(camera); //for lighting updates
    updateObjs(objects);
}


void Scene::updateObjs(std::list<Object> &objects) {
    for (std::list<Object>::iterator obj = objects.begin(); obj != objects.end(); obj++) {
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
            Behavior* b = (Behavior*)obj->getComponent(Component::BEHAVIOR);
            if (b != NULL) {
                for (std::list<behavior_function>::iterator b1 = b->actions.begin(); b1 != b->actions.end(); b1++) {
                    (*b1)(*obj);
                }
            }
            updateObjs((*obj).getChildren());
        glPopMatrix();
    }
}

void Scene::draw() {
    glPushMatrix();
        positionCamera(camera);
        drawObjs(objects);
    glPopMatrix();
}


void Scene::drawObjs(std::list<Object> &objects) {
    for (std::list<Object>::iterator obj = objects.begin(); obj != objects.end(); obj++) {
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

            Renderer* r = (Renderer*)obj->getComponent(Component::RENDERER);
            if (r != NULL) {
                r->render();
            }
            drawObjs((*obj).getChildren());
        glPopMatrix();
    }
}

std::list<Object>& Scene::getObjsList()
{
    return objects;
}


Scene::~Scene() {
    if (camera != NULL) delete camera;
    camera = NULL;
}
