#include <utils.h>
#include <scene.h>
#include <renderer.h>
#include <transform.h>
#include <behavior.h>
#include <camera.h>
#include <collider.h>
#include <physicssubsystem.h>
#include <inputsubsystem.h>
#include <graphicssubsystem.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Scene::positionCamera(bool projection) {
    viewMat = glm::mat4(1);
    glm::mat4 vpMat;

    if (projection) {
        glMatrixMode(GL_PROJECTION);
        GLfloat mat[16];
        glGetFloatv(GL_PROJECTION_MATRIX, mat);
        vpMat = glm::make_mat4(mat);
    } else {
        glLoadIdentity();
    }

    if (camera != NULL) {
        Object* parent = camera->owner();
        while (parent != NULL) {
            Transform &t = *Transform::get(*parent);

            viewMat = glm::rotate(viewMat, -t.rotation.z(), glm::vec3(0,0,1));
            viewMat = glm::rotate(viewMat, -t.rotation.y(), glm::vec3(0,1,0));
            viewMat = glm::rotate(viewMat, -t.rotation.x(), glm::vec3(1,0,0));
            viewMat = glm::translate(viewMat, glm::vec3(-t.position.x(), -t.position.y(), -t.position.z()));

            parent = parent->parent();
        }
    }

    vpMat = vpMat*viewMat;
    glLoadMatrixf(glm::value_ptr(vpMat));

    if (projection){
        glMatrixMode(GL_MODELVIEW);
    }
}

void Scene::init() {
    if (camera == NULL) {
        Object* cam = Object::Find("MainCamera");
        if (cam != NULL) {
            setCamera(Camera::get(*cam));
            camera->setup();
            printf("Found 'MainCamera' and set up the view projection.\n");
        } else {
            //printf("Did not find 'MainCamera' object!\n");
        }
    } else {
        //camera->setup();
    }
}


void Scene::setCamera(Camera *camera) {
    this->camera = camera;
}

void Scene::addObject(Object& object) {
    object.registerWithScene(this);
    for (std::list<Object*>::iterator obj = object.getChildren().begin(); obj != object.getChildren().end(); obj++) {
        (*obj)->registerWithScene(this);
    }
    objects.push_back(&object);
}

void Scene::update() {
    init();
    if (glIsEnabled(GL_LIGHTING)) {
        glPushMatrix();
            positionCamera(true);
            updateObjs(objects);
        glPopMatrix();
    } else {
        updateObjs(objects);
    }

    if(GraphicsSubsystem::isInit) {
        glm::vec2 mouse = InputSubsystem::getMousePos();
        glm::vec2 res(GraphicsSubsystem::width, GraphicsSubsystem::height);

        mouse = glm::vec2(2)*(mouse - res/glm::vec2(2))/res;
        mouse.y = -mouse.y;

        //Get the object under the mouse cursor
        Object *obj = raycastObject(mouse);
    }
}

void Scene::updateObjs(std::list<Object*> &objects) {
    for (std::list<Object*>::iterator obj = objects.begin(); obj != objects.end(); obj++) {
        glPushMatrix();
            Transform *t = Transform::get(**obj);
            if (t == NULL) {
                continue;
                glPopMatrix();
            } else {
                glTranslatef(t->position.x(),t->position.y(),t->position.z());
                glRotatef(t->rotation.x(),1.0,0.0,0.0);
                glRotatef(t->rotation.y(),0.0,1.0,0.0);
                glRotatef(t->rotation.z(),0.0,0.0,1.0);
                glScalef(t->scale.x(),t->scale.y(),t->scale.z());
            }
            Behavior *b = Behavior::get(**obj);
            if (b != NULL) {
                for (std::list<std::string>::iterator b1 = b->actions.begin(); b1 != b->actions.end(); b1++) {
                    Behavior::Execute(*b1, **obj);
                }
                for (std::list<std::string>::iterator b1 = b->actionsLocal.begin(); b1 != b->actionsLocal.end(); b1++) {
                    Behavior::Execute(*b1, **obj);
                }
            }
            updateObjs((**obj).getChildren());
        glPopMatrix();
    }
}

Object* Scene::raycastObject(const glm::vec2 &deviceCoords) {
    if(camera == NULL) return NULL;

    glm::mat4 invProjMat = glm::inverse(camera->projMat);
    glm::mat4 invViewMat = glm::inverse(viewMat);

    glm::vec4 ray_eye = invProjMat*
                            glm::vec4(deviceCoords.x, deviceCoords.y, -1, 1);
    ray_eye.z = -1;
    ray_eye.w = 0;

    glm::vec3 ray_world = glm::normalize(glm::vec3(invViewMat*ray_eye));
    glm::vec3 cameraOrig = glm::vec3(invViewMat*glm::vec4(0,0,0,1));

    return raycastObject(cameraOrig, ray_world);
}

Object* Scene::raycastObject(const glm::vec3 &origin, const glm::vec3 &dir) {
    float closestDist = 1.0/0.0;
    Object* closest = NULL;

    for(Object* obj : objects) {
        Collider *cm = Collider::get(*obj);
        if(cm != NULL ) {
            //reset testing colors
            Renderer::get(*obj)->material.diffuse = vec3f(1,0,0);
            float dist;

            if(cm->typeId() == Collider::BOX) {
                dist = PhysicsSubsystem::RayToBoxIntersection(origin, dir, *((BoxCollider*)cm));
            }

            if(dist < closestDist) {
                closest = obj;
                closestDist = dist;
            }
        }
    }

    //For testing
    if(closest != NULL) {
        glm::vec3 p = origin+closestDist*dir;
        //std::cout<<"Collision at distance="<<closestDist<<"\n\t"<<"point=("<<
        //    p.x<<","<<p.y<<","<<p.z<<")\n";
        Renderer::get(*closest)->material.diffuse = vec3f(0,1,0);
    }

    return closest;
}


void Scene::draw(bool useCamera) {
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);

    if (useCamera)
        positionCamera(true);
    drawObjs(objects);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}


void Scene::drawObjs(std::list<Object*> &objects) {
    for (std::list<Object*>::iterator obj = objects.begin(); obj != objects.end(); obj++) {
        glPushMatrix();
            Transform* t = Transform::get(**obj);
            if (t == NULL) {
                glPopMatrix();
                continue;
            } else {
                glTranslatef(t->position.x(),t->position.y(),t->position.z());
                glRotatef(t->rotation.x(),1.0,0.0,0.0);
                glRotatef(t->rotation.y(),0.0,1.0,0.0);
                glRotatef(t->rotation.z(),0.0,0.0,1.0);
                glScalef(t->scale.x(),t->scale.y(),t->scale.z());
            }

            Renderer* r = Renderer::get(**obj);
            if (r != NULL) {
                r->render();
            }
            drawObjs((*obj)->getChildren());
        glPopMatrix();
    }
}

std::list<Object*>& Scene::getObjsList()
{
    return objects;
}


Scene::~Scene() {
    if (camera != NULL) delete camera;
    camera = NULL;
}
