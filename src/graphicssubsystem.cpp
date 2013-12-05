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
std::map<std::string, Material::Texture*> GraphicsSubsystem::textureCache;

double GraphicsSubsystem::frameStart = 0;
double GraphicsSubsystem::counter = 0;
double GraphicsSubsystem::delta = 0;
int GraphicsSubsystem::width = 0;
int GraphicsSubsystem::height = 0;
int GraphicsSubsystem::fps = 0;
int GraphicsSubsystem::frames = 0;

bool GraphicsSubsystem::isInit = false;

GLuint GraphicsSubsystem::shadowMapTextures[3];
GLuint GraphicsSubsystem::shadowMapFramebuffer[3];
bool GraphicsSubsystem::shadowMappingEnabled = false;

int shadowMapSize = 256;
char fpsStr[8], shadowMapStr[24];

void GraphicsSubsystem::init(int argc, char* argv[])
{
    EventManager::RegisterCommand("exit", GraphicsSubsystem::shutdown);
    glutInit(&argc, argv);

    Behavior::Register("lightUpdate", Light::update);

    textureCache["empty"] = new Material::Texture(Material::Texture::EMPTY);
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
    printf("cleaning up texture cache...\n");
    for (std::map<std::string,Material::Texture*>::iterator tex = textureCache.begin(); tex != textureCache.end(); tex++) {
        if (tex->second != NULL) {
            delete tex->second;
            tex->second = NULL;
        }
    }

    NetworkSubsystem::disconnect();
    NetworkSubsystem::shutdown();

    glDeleteTextures(NUM_SHADOWMAPS, shadowMapTextures);
    glDeleteFramebuffers(NUM_SHADOWMAPS, shadowMapFramebuffer);

    glutLeaveMainLoop();
}

Material::Shader& GraphicsSubsystem::loadShader(std::string name) {
    if (shaderCache[name] == NULL) {
        shaderCache[name] = new Material::Shader(name);
    }
    return *shaderCache[name];
}


Material::Texture& GraphicsSubsystem::loadTexture(std::string name) {
    if (textureCache[name] == NULL) {
        textureCache[name] = new Material::Texture(2+textureCache.size(), name);
    }
    return *textureCache[name];
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
    isInit = true;

    glGenTextures(NUM_SHADOWMAPS, shadowMapTextures);
    glGenFramebuffers(NUM_SHADOWMAPS, shadowMapFramebuffer);
    Material::Shader &defaultShader = loadShader("default");
    glUseProgram(defaultShader.prog);
    for (int i = 0; i < NUM_SHADOWMAPS; i++) {
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, shadowMapTextures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, shadowMapSize, shadowMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        sprintf(shadowMapStr, "shadowMapTexture%d", i);
        GLint shadowMapTexture = glGetUniformLocation(defaultShader.prog, shadowMapStr);
        glUniform1i(shadowMapTexture, i);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFramebuffer[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTextures[i], 0);
        // No color output in the bound framebuffer, only depth.
        glDrawBuffer(GL_NONE);
        GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
            printf("framebuffer %d fucked (%d)\n", i, status);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDrawBuffer(GL_BACK);
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

void renderSceneDepthToTexture(GLuint framebufferHandle, Scene &scene, int s=shadowMapSize) {
    GLint lightingEnabled = glIsEnabled(GL_LIGHTING);
    if (lightingEnabled) glDisable(GL_LIGHTING);

    glBindFramebuffer(GL_FRAMEBUFFER, framebufferHandle);
    glViewport(0,0,s,s);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    NO_SHADER=true;
    scene.draw(false);
    NO_SHADER=false;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (lightingEnabled) glEnable(GL_LIGHTING);
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

            //temporary hack-fix for the broken first shadowmap
            renderSceneDepthToTexture(shadowMapFramebuffer[0], SceneManager::CurrentScene(), 1);

            for (int i = 0; i < NUM_SHADOWMAPS; i++) {
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
    int w = GraphicsSubsystem::width;
    int h = GraphicsSubsystem::height;
    glViewport(0,0,w,h);
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

    //need to fix the glitchy late lighting updates
    //caused by merging view transform into projection matrix
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
