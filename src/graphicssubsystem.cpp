#include "graphicssubsystem.h"
#include "scenemanager.h"
#include "utils.h"
#include "eventmanager.h"
#include "inputsubsystem.h"
#include <physicssubsystem.h>
#include <networksubsystem.h>

#include <unistd.h>
#include <string>
#include <cstdio>

std::map<std::string, Material::Shader*> GraphicsSubsystem::shaderCache;
double GraphicsSubsystem::frameStart = 0;
double GraphicsSubsystem::counter = 0;
double GraphicsSubsystem::delta = 0;
int GraphicsSubsystem::width = 0;
int GraphicsSubsystem::height = 0;
int GraphicsSubsystem::fps = 0;
int GraphicsSubsystem::frames = 0;
char GraphicsSubsystem::fpsStr[8];

void GraphicsSubsystem::init(int argc, char* argv[])
{
    EventManager::RegisterCommand("exit", GraphicsSubsystem::shutdown);
    glutInit(&argc, argv);
}

void GraphicsSubsystem::shutdown()
{
    printf("%s\n", "cleaning up shader cache...");
    for (std::map<std::string,Material::Shader*>::iterator shad = shaderCache.begin(); shad != shaderCache.end(); shad++) {
        if (shad->second != NULL) {
            delete shad->second;
            shad->second = NULL;
        }
    }
    glutLeaveMainLoop();
}

Material::Shader& GraphicsSubsystem::loadShader(std::string name) {
    if (shaderCache[name] == NULL) {
        shaderCache[name] = new Material::Shader(name);
    }
    return *shaderCache[name];
}

void GraphicsSubsystem::createWindow(int x, int y, int w, int h, const char* title)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(w, h);
    width = w;
    height = h;
    glutInitWindowPosition(x,y);
    glutCreateWindow(title);

    GLint err = glewInit();
    if (err != GLEW_OK) {
        printf("GLEW initialization failure: %s", glewGetErrorString(err));
        exit(-1);
    }

    glutDisplayFunc(GraphicsSubsystem::draw);
    glutIdleFunc(GraphicsSubsystem::idle);

    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
}

void GraphicsSubsystem::zBufferEnabled(bool enabled)
{
    if (enabled)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
}

void GraphicsSubsystem::draw()
{
    // Clear screen (TODO: add per-scene render settings?)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    PhysicsSubsystem::PerformPhysicsChecks();
    SceneManager::testScene.draw();

    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0.0, width, 0.0, height);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glLoadIdentity();
        glColor3f(0.0, 1.0, 0.0);
        glRasterPos2f(10.0, 10.0);
        sprintf(fpsStr, "FPS: %d", fps);
        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned const char*)fpsStr);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_LIGHTING);

    glutSwapBuffers();
}

void GraphicsSubsystem::idle()
{
    if (frames == 0) {
        counter = Utils::time();
    }
    double needed = frameStart+(1000.0/MAX_FPS);
    if (needed >= Utils::time()) {
        double d = (needed-Utils::time())*1000;
        usleep(d);
    }

    if (counter+1000 >= Utils::time()) {
        frames++;
    } else {
        fps = frames;
        frames = 0;
    }

    NetworkSubsystem::parseIncomingPackets();

    InputSubsystem::update();
    EventManager::ParseEvents();
    delta = (Utils::time()-frameStart)/1000;

    SceneManager::CurrentScene().update();

    frameStart = Utils::time();
    glutPostRedisplay();
}

void GraphicsSubsystem::run()
{
    frameStart = Utils::time();
    glutMainLoop();
}
