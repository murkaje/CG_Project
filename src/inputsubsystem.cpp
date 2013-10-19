#include "inputsubsystem.h"
#include "eventmanager.h"

#include <GL/freeglut.h>
#include <fstream>
#include <iostream>

std::map<std::string,int> InputSubsystem::keyMap;
std::map<int,void (*)()> InputSubsystem::bindMap;

void InputSubsystem::init() {
    glutKeyboardFunc(InputSubsystem::keys);
    glutSpecialFunc(InputSubsystem::specialKeys);

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
            bind(keyMap[vars[0]], EventManager::GetCommand(vars[1]));
        }
        getline(bindFile, line);
    }
}

//This might need rethinking
void InputSubsystem::mapKeys() {
    keyMap["ESC"] = 27;
    keyMap["ENTER"] = 13;
    keyMap["SPACE"] = ' ';
}

void InputSubsystem::keys(unsigned char key, int x, int y) {
    try {
        EventManager::QueEvent(bindMap.at(key));
    } catch(std::exception e) {}
}

void InputSubsystem::specialKeys(int key, int x, int y) {

}

void InputSubsystem::bind(int key, void (*func)()) {
    bindMap[key] = func;
}
