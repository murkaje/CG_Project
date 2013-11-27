#ifndef GRAPHICSSUBSYSTEM_H
#define GRAPHICSSUBSYSTEM_H

#include <material.h>

#include <map>
#include <string>

class GraphicsSubsystem {
private:
    static std::map<std::string, Material::Shader*> shaderCache;

    static char fpsStr[8], shadowMapStr[24];

    static void drawFPS();

    static double frameStart, counter;

    static int frames;

    static void draw();

    static void idle();

    static GLuint shadowMapTextures[3];
    static GLuint shadowMapFramebuffer[3];
public:
    static bool shadowMappingEnabled;

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
