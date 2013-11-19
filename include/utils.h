#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <cstdio>
#include "vector3f.h"
#include "RPC3.h"
#include "BitStream.h"
#include "RakNetTypes.h"

#define v3f vector3f

class Object;

class Utils {
public:
    static double time();
    static std::string load(const char *filename);
};

void Instantiate(Object &obj, RakNet::RPC3 *rpcFromNetwork = 0);

//void InitScene(RakNet::RPC3 *rpcFromNetwork = 0);

#endif
