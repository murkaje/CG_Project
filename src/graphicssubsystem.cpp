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
GLuint GraphicsSubsystem::shadowMapFramebuffer[3];
bool GraphicsSubsystem::shadowMappingEnabled = false;

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
    glDeleteFramebuffers(3, shadowMapFramebuffer);

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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_TEXTURE_2D);
    glGenTextures(3, shadowMapTextures);
    glGenFramebuffers(3, shadowMapFramebuffer);
    Material::Shader &defaultShader = loadShader("default");
    glUseProgram(defaultShader.prog);
    for (int i = 0; i < 3; i++) {
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, shadowMapTextures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        sprintf(shadowMapStr, "shadowMapTexture%d", i);
        GLint shadowMapTexture = glGetUniformLocation(defaultShader.prog, shadowMapStr);
        glUniform1i(shadowMapTexture, i);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

        //shadowMapFramebuffer[i] = 0;
        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFramebuffer[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTextures[i], 0);
        // No color output in the bound framebuffer, only depth.
        glDrawBuffer(GL_NONE);
        GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
            printf("framebuffer %d fucked (%d)\n", i, status);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glUseProgram(0);
}

void GraphicsSubsystem::zBufferEnabled(bool enabled)
{
    if (enabled) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
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

void renderSceneDepthToTexture(GLuint framebufferHandle, Scene &scene, bool toScreen=false) {
    int w = GraphicsSubsystem::width;
    int h = GraphicsSubsystem::height;

    GLint lightingEnabled = glIsEnabled(GL_LIGHTING);
    if (lightingEnabled) glDisable(GL_LIGHTING);           // Turn off lighting in the shader

    glBindFramebuffer(GL_FRAMEBUFFER, framebufferHandle);
    glViewport(0,0,w,h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.draw(false);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

double angle = 120.0 * PI / 180.0;
double *rotationMatrix = new double[9] {
             cos(angle),    0.0,    sin(angle),
             0.0,           1.0,    0.0,
             -sin(angle),   0.0,    cos(angle)};

void GraphicsSubsystem::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderCache["default"]->prog);
    GLint shad = glGetUniformLocation(shaderCache["default"]->prog, "shadows_enabled");
    glUniform1i(shad, shadowMappingEnabled);
    glUseProgram(0);

    if (shadowMappingEnabled) {
        std::string pName = std::string("player-")+NetworkSubsystem::rpc->GetRakPeer()->GetMyBoundAddress().ToString();

        Object *player = Object::Find(pName);
        if (player != NULL) {
            vec3f dir = vec3f(1,0,0);

            for (int i = 0; i < 3; i++) {
            float shadowMatrix[16];
                if (i > 0){
                    dir = vec3f(rotationMatrix[0*3+0]*dir.x()+rotationMatrix[0*3+1]*dir.y()+rotationMatrix[0*3+2]*dir.z(),
                                rotationMatrix[1*3+0]*dir.x()+rotationMatrix[1*3+1]*dir.y()+rotationMatrix[1*3+2]*dir.z(),
                                rotationMatrix[2*3+0]*dir.x()+rotationMatrix[2*3+1]*dir.y()+rotationMatrix[2*3+2]*dir.z());
                }
                setProjectionFromPoint(Transform::get(*player)->position+vec3f(0,0.25,0), dir);
                renderSceneDepthToTexture(shadowMapFramebuffer[i], SceneManager::CurrentScene());
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
