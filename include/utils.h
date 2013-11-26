#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <cstdio>
#include "vec.h"
#include "RPC3.h"
#include "BitStream.h"
#include "RakNetTypes.h"

class Object;

namespace Utils {
    double time();
    std::string load(const char *filename);
};

void Instantiate(Object &obj);

void RemoteInstantiate(Object &obj, RakNet::RPC3 *rpcFromNetwork = 0);

void Destroy(Object &obj);

void RemoteDestroy(RakNet::RakString objNameRs, RakNet::RPC3 *rpcFromNetwork = 0);

#endif
