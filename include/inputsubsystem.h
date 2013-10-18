#ifndef INPUTSUBSYSTEM_H
#define INPUTSUBSYSTEM_H
#include <map>
#include <string>

class InputSubsystem {
private:
    static void keys(unsigned char key, int x, int y);

    static void specialKeys(int key, int x, int y);
    static void specialUpKeys(int key, int x, int y);

public:
    const static int KEY_ESCAPE = 27;
    const static int KEY_ENTER = 13;
    static std::map<std::string,bool> keyDown;

    static void init();
};

#endif
