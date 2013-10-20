#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <scene.h>

class SceneManager
{
public:
    static Scene testScene;

    static Scene& CurrentScene();
};

#endif
