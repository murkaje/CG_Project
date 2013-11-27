#include "graphicssubsystem.h"
#include "scenemanager.h"
#include "utils.h"
#include "eventmanager.h"
#include "inputsubsystem.h"
#include <physicssubsystem.h>
#include <networksubsystem.h>
#include "behavior.h"
#include "light.h"
#include "camera.h"

#include <unistd.h>
#include <string>
#include <cstdio>

#define PI 3.14159265

std::map<std::string, Material::Shader*> GraphicsSubsystem::shaderCache;
double GraphicsSubsystem::frameStart = 0;
double GraphicsSubsystem::counter = 0;
double GraphicsSubsystem::delta = 0;
int GraphicsSubsystem::width = 0;
int GraphicsSubsystem::height = 0;
int GraphicsSubsystem::fps = 0;
int GraphicsSubsystem::frames = 0;
char GraphicsSubsystem::fpsStr[8];
char GraphicsSubsystem::shadowMapStr[24];

GLuint GraphicsSubsystem::shadowMapTextures[3];

void GraphicsSubsystem::init(int argc, char* argv[])
{
    EventManager::RegisterCommand("exit", GraphicsSubsystem::shutdown);
    glutInit(&argc, argv);

    Behavior::Register("lightUpdate", Light::update);

}

void GraphicsSubsystem::shutdown()
{
    printf("cleaning up shader cache...\n");
    for (std::map<std::string,Material::Shader*>::iterator shad = shaderCache.begin(); shad != shaderCache.end(); shad++) {
        if (shad->second != NULL) {
            delete shad->second;
            shad->second = NULL;
        }
    }
    NetworkSubsystem::disconnect();
    NetworkSubsystem::shutdown();

    glDeleteTextures(3, shadowMapTextures);

    glutLeaveMainLoop();
}

Material::Shader& GraphicsSubsystem::loadShader(std::string name) {
    if (!NetworkSubsystem::isServer && shaderCache[name] == NULL) {
        shaderCache[name] = new Material::Shader(name);
    }
    return *shaderCache[name];
}

void GraphicsSubsystem::createWindow(int x, int y, int w, int h, const char* title)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(w, h);
    width = w;
    height = h;
    glutInitWindowPosition(x,y);
    glutCreateWindow(title);

    GLint err = glewInit();
    if (err != GLEW_OK) {
        printf("GLEW initialization failure: %s\n", glewGetErrorString(err));
        exit(-1);
    }

    glutDisplayFunc(GraphicsSubsystem::draw);

    glutIdleFunc(GraphicsSubsystem::idle);

    glEnable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_TEXTURE_2D);
    glGenTextures(3, shadowMapTextures);
    Material::Shader &defaultShader = loadShader("default");
    glUseProgram(defaultShader.prog);
    for (int i = 0; i < 3; i++) {
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, shadowMapTextures[i]);
        sprintf(shadowMapStr, "shadowMapTexture%d", i);
        GLint shadowMapTexture = glGetUniformLocation(defaultShader.prog, shadowMapStr);
        glUniform1i(shadowMapTexture, i);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    glUseProgram(0);
}

void GraphicsSubsystem::zBufferEnabled(bool enabled)
{
    if (enabled) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void setProjectionFromPoint(vec3f pos, vec3f dir) {
    glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(104.83, 4/3.0, 0.1, 20);
        gluLookAt(pos.x(), pos.y(), pos.z(),
                  pos.x()+dir.x(), pos.y()+dir.y(), pos.z()+dir.z(),
                  0, 1, 0);
    glMatrixMode(GL_MODELVIEW);
}

void renderSceneDepthToTexture(GLuint textureHandle, Scene &scene, bool toScreen=false) {
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);

    float* depth_data = (float*)malloc(w*h*sizeof(float)); // Initialize array for depth data
    GLint lightingEnabled = glIsEnabled(GL_LIGHTING);
    if (lightingEnabled) glDisable(GL_LIGHTING);           // Turn off lighting in the shader

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);   // Disable writing to screen

    scene.draw(false);

    // Now read out the depth buffer data and copy it to the given texture

    // Note: what we are doing here is not the most efficient way to do it.
    // In fact, in modern OpenGL you would rather render directly to the texture by creading a separate framebuffer object.
    // (see e.g. http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/ and
    // https://developer.nvidia.com/sites/default/files/akamai/gamedev/docs/opengl_rendertexture.pdf)
    // However, this might not work on some older machines (and it requires the introduction of a bunch of
    // OpenGL-specific commands, that are not in principle the topic of this practice session nor this course per se).
    glReadPixels(0, 0, w, h, GL_DEPTH_COMPONENT, GL_FLOAT, depth_data);
    glBindTexture(GL_TEXTURE_2D, textureHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, w, h, 0, GL_LUMINANCE, GL_FLOAT, depth_data);

    // NB: If you want to debug such code, the following line flushes the depth array to the screen
    if (toScreen) glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); glDrawPixels(w, h, GL_LUMINANCE, GL_FLOAT, depth_data);

    free(depth_data);   // Release memory

    // Switch writing to screen back on, clear the depth buffer that we messed up
    glClear(GL_DEPTH_BUFFER_BIT);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    if (lightingEnabled) glEnable(GL_LIGHTING); // Enable lighting again
}

void GraphicsSubsystem::drawFPS() {
    GLint lightingEnabled = glIsEnabled(GL_LIGHTING);
    if (lightingEnabled) glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0.0, width, 0.0, height);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glLoadIdentity();
        glColor3f(0.0, 1.0, 0.0);
        glRasterPos2f(10.0, 10.0);
        sprintf(fpsStr, "FPS: %d", fps);
        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned const char*)fpsStr);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    if (lightingEnabled) glEnable(GL_LIGHTING);
}

void GraphicsSubsystem::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int shadows = 0;
    if (shadows) {
        std::string pName = std::string("player-")+NetworkSubsystem::rpc->GetRakPeer()->GetMyBoundAddress().ToString();
        Object *player = Object::Find(pName);
        if (player != NULL) {
            float angle = 120.0 * PI / 180.0;
            float *rotationMatrix = new float[9] {
             cos(angle),    0.0,    sin(angle),
             0.0,           1.0,    0.0,
             -sin(angle),   0.0,    cos(angle),
            };
            vec3f dir = vec3f(1,0,0);

            for (int i = 0; i < 3; i++) {
            float shadowMatrix[16];
                if (i > 0){
                    dir = vec3f(rotationMatrix[0*3+0]*dir.x()+rotationMatrix[0*3+1]*dir.y()+rotationMatrix[0*3+2]*dir.z(),
                                rotationMatrix[1*3+0]*dir.x()+rotationMatrix[1*3+1]*dir.y()+rotationMatrix[1*3+2]*dir.z(),
                                rotationMatrix[2*3+0]*dir.x()+rotationMatrix[2*3+1]*dir.y()+rotationMatrix[2*3+2]*dir.z());
                }
                setProjectionFromPoint(Transform::get(*player)->position+vec3f(0,1,0), dir);
                renderSceneDepthToTexture(shadowMapTextures[i], SceneManager::CurrentScene());
                glGetFloatv(GL_PROJECTION_MATRIX, shadowMatrix);
                glUseProgram(shaderCache["default"]->prog);
                sprintf(shadowMapStr, "shadowMapMatrix[%d]", i);
                GLint shadowMapMatrix = glGetUniformLocation(shaderCache["default"]->prog, shadowMapStr);
                glUniformMatrix4fv(shadowMapMatrix, 1, GL_FALSE, shadowMatrix);
                glUseProgram(0);
            }
        }
    }
    if (SceneManager::CurrentScene().camera != NULL) {
        SceneManager::CurrentScene().camera->setup();
    }
    SceneManager::CurrentScene().draw();

    drawFPS();

    glutSwapBuffers();
}

void GraphicsSubsystem::idle()
{
    if (frames == 0) {
        counter = Utils::time();
    }

    if (counter+1000 >= Utils::time()) {
        frames++;
    } else {
        fps = frames;
        frames = 0;
    }
    if (frames%5 == 0) {
        NetworkSubsystem::synchronizeCurrentScene();
    }

    InputSubsystem::update();
    EventManager::ParseEvents();

    PhysicsSubsystem::PerformPhysicsChecks();
    Light::lightsCache.reset();
    SceneManager::CurrentScene().update();

    if (frames%1 == 0) {
        NetworkSubsystem::parseIncomingPackets();
    }
    double needed = frameStart+(1000.0/MAX_FPS);
    if (needed >= Utils::time()) {
        double d = (needed-Utils::time())*1000;
        usleep(d);
    }
    delta = (Utils::time()-frameStart)/1000;
    frameStart = Utils::time();

    glutPostRedisplay();
}

void GraphicsSubsystem::run()
{
    frameStart = Utils::time();
    glutMainLoop();
}
