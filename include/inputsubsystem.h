#ifndef INPUTSUBSYSTEM_H
#define INPUTSUBSYSTEM_H

class InputSubsystem {
private:
    static void keys(unsigned char key, int x, int y);

    static void specialKeys(int key, int x, int y);

public:
    const static int KEY_ESCAPE = 27;
    const static int KEY_ENTER = 13;

    static void init();

};

#endif
