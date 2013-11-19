#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <cstdio>
#include "vec.h"

class Utils {
public:
    static double time();
    static std::string load(const char *filename);
};

#endif
