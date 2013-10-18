#include "inputsubsystem.h"
#include "graphicssubsystem.h"

#include <GL/freeglut.h>

std::map<std::string,bool> InputSubsystem::keyDown;

void InputSubsystem::init() {
    glutKeyboardFunc(InputSubsystem::keys);
    glutSpecialFunc(InputSubsystem::specialKeys);
    glutSpecialUpFunc(InputSubsystem::specialUpKeys);
}

void InputSubsystem::keys(unsigned char key, int x, int y) {
    if (key == InputSubsystem::KEY_ESCAPE || key == ' ')
        GraphicsSubsystem::shutdown();
}

void InputSubsystem::specialKeys(int key, int x, int y) {
    if(key == GLUT_KEY_UP)
    {
        keyDown["UP"] = true;
    }
    else if(key == GLUT_KEY_DOWN)
    {
        keyDown["DOWN"] = true;
    }
    else  if(key == GLUT_KEY_RIGHT)
    {
        keyDown["RIGHT"] = true;
    }
    else  if(key == GLUT_KEY_LEFT)
    {
        keyDown["LEFT"] = true;
    }
}

void InputSubsystem::specialUpKeys(int key, int x, int y) {
    if(key == GLUT_KEY_UP)
    {
        keyDown["UP"] = false;
    }
     else if(key == GLUT_KEY_DOWN)
    {
        keyDown["DOWN"] = false;
    }
    else  if(key == GLUT_KEY_RIGHT)
    {
        keyDown["RIGHT"] = false;
    }
    else  if(key == GLUT_KEY_LEFT)
    {
        keyDown["LEFT"] = false;
    }
}




