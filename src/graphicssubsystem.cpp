#include "graphicssubsystem.h"
#include "scenemanager.h"
#include "utils.h"

#include <unistd.h>

#include <GL/freeglut.h>

double GraphicsSubsystem::frameStart = 0;
double GraphicsSubsystem::delta = 0;

void GraphicsSubsystem::init(int argc, char* argv[]) {
    glutInit(&argc, argv);
}

void GraphicsSubsystem::shutdown() {
    glutLeaveMainLoop();
}

void GraphicsSubsystem::createWindow(int x, int y, int w, int h, const char* title) {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(w, h);
    glutInitWindowPosition(x,y);
    glutCreateWindow(title);

    glutDisplayFunc(GraphicsSubsystem::draw);
    glutIdleFunc(GraphicsSubsystem::idle);

    // Set up perspective
    glMatrixMode(GL_PROJECTION);
        gluPerspective(45, 1, 0.5, 100);
    glMatrixMode(GL_MODELVIEW);

    // Enable basic lighting
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
}

void GraphicsSubsystem::zBufferEnabled(bool enabled) {
    if (enabled) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void GraphicsSubsystem::draw() {
    // Clear screen (TODO: add per-scene render settings?)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up view (TODO: create camera object, add it to scene and set up in the scene draw() call)
    glLoadIdentity();
    gluLookAt(6, 6, 6, 0, 0, 0, 0, 1, 0);

    SceneManager::testScene.draw();

    glutSwapBuffers();
}

void GraphicsSubsystem::idle() {
    double needed = frameStart+(1000.0/MAX_FPS);
    if (needed >= Utils::time()) {
        double d = (needed-Utils::time())*1000;
        usleep(d);
    }

    delta = (Utils::time()-frameStart)/1000;

    SceneManager::testScene.update();

    frameStart = Utils::time();
    glutPostRedisplay();
}

void GraphicsSubsystem::run() {
    frameStart = Utils::time();
    glutMainLoop();
}
