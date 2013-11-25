#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <scene.h>
#include <vec.h>
#include <string>

class SceneManager
{
public:
    static Scene testScene;

    static Scene& CurrentScene();
};

#endif
