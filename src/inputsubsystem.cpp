#include "inputsubsystem.h"
#include "graphicssubsystem.h"

#include <GL/freeglut.h>

void InputSubsystem::init() {
    glutKeyboardFunc(InputSubsystem::keys);
    glutSpecialFunc(InputSubsystem::specialKeys);
}

void InputSubsystem::keys(unsigned char key, int x, int y) {
    if (key == InputSubsystem::KEY_ESCAPE || key == ' ')
        GraphicsSubsystem::shutdown();
}

void InputSubsystem::specialKeys(int key, int x, int y) {

}
