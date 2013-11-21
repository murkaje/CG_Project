#include "utils.h"
#include "object.h"
#include "scenemanager.h"
#include "synchronizer.h"
#include "networksubsystem.h"

#include <stdexcept>
#include <cerrno>
#include <iostream>
#include <fstream>

double Utils::time() {
    return glutGet(GLUT_ELAPSED_TIME);
}

std::string Utils::load(const char *filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) return std::string((std::istreambuf_iterator<char>(in)),
                                std::istreambuf_iterator<char>());
    else throw(errno);
}

void Instantiate(Object &obj, RakNet::RPC3 *rpcFromNetwork) {
    if (rpcFromNetwork == 0) {
        printf("Locally placed %s in current scene\n", obj.name.c_str());
        SceneManager::CurrentScene().addObject(obj);
    } else {
        printf("Deserialized %s and placed it in current scene\n", obj.name.c_str());
        SceneManager::CurrentScene().addObject(Object::copy(obj));
    }
}
