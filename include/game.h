#ifndef GAME_H
#define GAME_H

#include <string>
#include <list>
#include <map>

#include <vec.h>
#include <glm/glm.hpp>

class Object;

class Game
{
private:
    static Game* instance;
protected:
public:

    static void init();

    static Game& get();

    struct Player {
        glm::vec3 moveVec;

        Object *playerObj;

        float movePerSecond = 3;
        float rotationSpeed = 90;

        std::string name;

        Player(std::string name);

        static Object* createCharacter(std::string name, std::string addr);
    } localPlayer;

    std::map<std::string, Player*> connectedPlayers;

    static void attachCamera(Object &obj);

    Game();
    ~Game();
};


void moveLeft();
void moveRight();
void moveBackward();
void moveForward();

void rotateLeft();
void rotateRight();

#endif
