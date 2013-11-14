#ifndef UTILS_H
#define UTILS_H

#include <windef.h>

#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <cstdio>
#include "vector3f.h"

#define v3f vector3f

class Utils {
public:
    static double time();
    static std::string load(const char *filename);
};

#endif
