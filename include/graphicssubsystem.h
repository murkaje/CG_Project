#ifndef GRAPHICSSUBSYSTEM_H
#define GRAPHICSSUBSYSTEM_H

#include <material.h>

#include <map>
#include <string>

#define NUM_SHADOWMAPS 3

class GraphicsSubsystem {
private:
    static std::map<std::string, Material::Shader*> shaderCache;
    static std::map<std::string, Material::Texture*> textureCache;

    static void drawFPS();

    static double frameStart, counter;

    static int frames;

    static void draw();

    static void idle();

    static GLuint shadowMapTextures[NUM_SHADOWMAPS];
    static GLuint shadowMapFramebuffer[NUM_SHADOWMAPS];
public:
    static bool shadowMappingEnabled, isInit;

    static Material::Shader& loadShader(std::string name);
    static Material::Texture& loadTexture(std::string name);


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
