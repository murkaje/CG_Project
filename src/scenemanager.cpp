#include "scenemanager.h"
#include "object.h"
#include "component.h"
#include "collider.h"
#include "renderer.h"
#include "behavior.h"
#include "transform.h"
#include "synchronizer.h"
#include "light.h"
#include "camera.h"

#include "funcdefs.h"

Scene SceneManager::testScene = Scene();

Scene& SceneManager::CurrentScene() {
    return testScene;
}
