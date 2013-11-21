#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <scene.h>
#include <vec.h>
#include <string>

class SceneManager
{
public:

    static vec3f moveVec;
    static const int movePerSecond = 2;

    static Scene testScene;

    static void createTestScene();

    static Object* createPlayerCharacter(std::string name);

    static Scene& CurrentScene();
};

#endif
