#ifndef INPUTSUBSYSTEM_H
#define INPUTSUBSYSTEM_H

#include <map>
#include <string>

class InputSubsystem {
    static std::map<std::string,int> keyMap;    //Maps key strings to key values
    static std::map<int,void (*)()> bindMap;    //Maps key values to functions

    static void keys(unsigned char key, int x, int y);
    static void specialKeys(int key, int x, int y);

public:
    const static int KEY_ESCAPE = 27;
    const static int KEY_ENTER = 13;

    static void init();
    static void mapKeys();
    static void bind(int key, void (*func)());
};

#endif
