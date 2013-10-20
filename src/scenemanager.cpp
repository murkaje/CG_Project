#include "scenemanager.h"

Scene SceneManager::testScene = Scene();

Scene& SceneManager::CurrentScene() {
    return testScene;
}
