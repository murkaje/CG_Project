#include "utils.h"

#include <GL/freeglut.h>

double Utils::time() {
    return glutGet(GLUT_ELAPSED_TIME);
}
