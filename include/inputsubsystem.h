#ifndef INPUTSUBSYSTEM_H
#define INPUTSUBSYSTEM_H

#include <map>
#include <string>
#include <glm/glm.hpp>

class InputSubsystem {
    static glm::vec2 mousePos;
    static std::map<std::string,int> keyMap;    //Maps key strings to key values
    static std::map<int,void (*)()> bindMap;    //Maps key values to functions
    static std::map<int, std::string> keyNameMap;      //Maps key values to strings

    static void keyDown(unsigned char key, int x, int y);
    static void keyUp(unsigned char key, int x, int y);
    static void specialKeys(int key, int x, int y);
    static void specialUpKeys(int key, int x, int y);
    static void mouseMove(int x, int y);

public:
    static std::map<int, bool> keyState;

    static void update();
    static void init();
    static void mapKeys();
    static void bind(int key, void (*func)());

    static glm::vec2 getMousePos();
};

#endif
