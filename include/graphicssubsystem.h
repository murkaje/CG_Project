#ifndef GRAPHICSSUBSYSTEM_H
#define GRAPHICSSUBSYSTEM_H

#include <material.h>

#include <map>
#include <string>

class GraphicsSubsystem {
private:
    static std::map<std::string, Material::Shader*> shaderCache;

    static char fpsStr[8];

    static double frameStart, counter;

    static int frames;

    static void draw();

    static void idle();

public:

    static Material::Shader& loadShader(std::string name);

    static const int MAX_FPS = 60;
    static double delta;
    static int width, height, fps;

    static void init(int argc, char* argv[]);

    static void createWindow(int x, int y, int w, int h, const char* title);

    static void zBufferEnabled(bool enabled);

    static void run();

    static void shutdown();

};

#endif
