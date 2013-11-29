#include "inputsubsystem.h"
#include "eventmanager.h"

#include <fstream>
#include <iostream>

#include <utils.h>

glm::vec2 InputSubsystem::mousePos;
std::map<std::string, int> InputSubsystem::keyMap;
std::map<int, std::string> InputSubsystem::keyNameMap;
std::map<int, void (*)()> InputSubsystem::bindMap;
std::map<int, bool> InputSubsystem::keyState;

void InputSubsystem::init() {
    glutKeyboardFunc(InputSubsystem::keyDown);
    glutKeyboardUpFunc(InputSubsystem::keyUp);
    glutSpecialFunc(InputSubsystem::specialKeys);
    glutSpecialUpFunc(InputSubsystem::specialUpKeys);
    glutPassiveMotionFunc(InputSubsystem::mouseMove);

    mapKeys();

    //Move this to a separate class once binds, commands and cvars are done
    //and replaced with optimized versions
    std::ifstream bindFile("binds.cfg");
    if(!bindFile) return;

    std::string line;
    getline(bindFile, line);

    //Needs better whitespace and inline comment handling
    while(!bindFile.eof()) {
        std::vector<std::string> vars;
        size_t begin = 0;
        for(size_t cur=0; cur<=line.size(); cur++) {
            if(line[cur] == '/' && cur != 0 && line[cur-1] == '/') {
                vars.push_back(line.substr(begin, cur-begin));
                break;
            }
            if(line[cur] == ' ' || cur==line.size()) {
                vars.push_back(line.substr(begin, cur-begin));
                begin = cur+1;
            }
        }
        if(vars.size() == 2) {
            //Might be better to evaluate GetCommand later to
            //allow command registrations after init
            bind(keyMap[vars[0]], EventManager::GetCommand(vars[1]));
        }
        getline(bindFile, line);
    }
}

//Eventually need unified key map as regular and special key codes overlap
//Possibly indexed strings and int <-> int lookup tables.
void InputSubsystem::mapKeys() {
    keyMap["ESC"] = 27;
    keyMap["ENTER"] = 13;
    keyMap["SPACE"] = ' ';
    keyMap["W"] = 'w';
    keyMap["A"] = 'a';
    keyMap["S"] = 's';
    keyMap["D"] = 'd';
}

void InputSubsystem::update() {
    std::map<int,bool>::const_iterator iter=keyState.begin();
    while(iter != keyState.end()) {
        if(iter->second) {
            try {
                EventManager::QueEvent(bindMap.at(iter->first));
            } catch(std::exception e) {}
        }
        iter++;
    }
}

void InputSubsystem::keyDown(unsigned char key, int x, int y) {
    keyState[key] = true;
}

void InputSubsystem::keyUp(unsigned char key, int x, int y) {
    keyState[key] = false;
}

void InputSubsystem::specialKeys(int key, int x, int y) {
    keyState[key+256] = true;
}

void InputSubsystem::specialUpKeys(int key, int x, int y) {
    keyState[key+256] = false;
}

void InputSubsystem::mouseMove(int x, int y) {
    mousePos.x = x;
    mousePos.y = y;
}

void InputSubsystem::bind(int key, void (*func)()) {
    if(func != NULL) {
        bindMap[key] = func;
    }
}

glm::vec2 InputSubsystem::getMousePos() {
    return mousePos;
}
