#include "utils.h"
#include "object.h"
#include "scenemanager.h"
#include "synchronizer.h"
#include "networksubsystem.h"

#include "game.h"

#include <stdexcept>
#include <cerrno>
#include <iostream>
#include <fstream>

#include <SOIL/SOIL.h>

double Utils::time() {
    return glutGet(GLUT_ELAPSED_TIME);
}

std::string Utils::load(const char *filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) return std::string((std::istreambuf_iterator<char>(in)),
                                std::istreambuf_iterator<char>());
    else throw(errno);
}

void Utils::loadTexture(GLuint &tex, const char* filename) {
    /* load an image file directly as a new OpenGL texture */
    tex = SOIL_load_OGL_texture
        (
            filename,
            SOIL_LOAD_AUTO,
            tex,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
        );
}

void Instantiate(Object &obj) {
    SceneManager::CurrentScene().addObject(obj);
    printf("Placed '%s'(%s) in current scene\n", obj.name.c_str(), obj.tag.c_str());
    //temporary hack to add camera locally
    RakNet::RakPeerInterface* peer = NetworkSubsystem::rpc->GetRakPeer();
    std::string addr;
    if(peer != NULL) {
        addr = peer->GetMyBoundAddress().ToString();
    }

    if (addr == obj.tag) {
        if (obj.tag.compare(addr) == 0 && Object::Find(&obj, "MainCamera") == NULL) {
            std::cout<<"attach camera\n";
            Game::attachCamera(obj);
            Game::get().localPlayer.playerObj = &obj;
        }
    }
}

void RemoteInstantiate(Object &obj, RakNet::RPC3 *rpcFromNetwork) {
    if (rpcFromNetwork == 0) {
        Instantiate(obj);
    } else {
        printf("Deserialized '%s'\n", obj.name.c_str());
        //instantiating a copy of the received object since the original will be popped
        //from the stack and the reference will point to something arbitrary in memory
        Instantiate(Object::copy(obj));
    }
}

void Destroy(Object &obj) {
    printf("Destroyed %s\n", obj.name.c_str());
    SceneManager::CurrentScene().getObjsList().remove(&obj);
    delete &obj;
}

void RemoteDestroy(RakNet::RakString objNameRs, RakNet::RPC3 *rpcFromNetwork) {
    const char* objName = objNameRs.C_String();
    if (rpcFromNetwork == 0) {
        printf("Local request to destroy '%s' from current scene\n", objName);
        Object *obj = Object::Find(objName);
        if (obj != NULL) {
            Destroy(*obj);
        } else {
            printf("Object '%s' does not exist in current scene", objName);
        }
    } else {
        printf("Remote request to destroy '%s' from current scene\n", objName);
        Object *obj = Object::Find(objName);
        if (obj != NULL) {
            Destroy(*obj);
        } else {
            printf("Object '%s' does not exist in current scene", objName);
        }
    }
}
