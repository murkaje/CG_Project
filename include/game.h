#ifndef GAME_H
#define GAME_H

#include <string>
#include <list>
#include <map>

#include <vec.h>

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
        vec3f moveVec;

        Object *playerObj;

        int movePerSecond = 2;

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
