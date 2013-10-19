#include "eventmanager.h"

std::queue<void (*)()> EventManager::eventQueue;
std::map<std::string, void (*)()> EventManager::cmdMap;

void EventManager::RegisterCommand(const std::string &name, void (*func)()) {
    if(func != NULL) {
        cmdMap[name] = func;
    }
}

void EventManager::QueEvent(void (*func)()) {
    if(func != NULL) {
        eventQueue.push(func);
    }
}

void EventManager::ParseEvents() {
    while(!eventQueue.empty()) {
        eventQueue.front()();
        eventQueue.pop();
    }
}

func_ptr EventManager::GetCommand(const std::string &name) {
    func_ptr func = NULL;
    try {
        func = cmdMap.at(name);
    } catch(std::exception e) {}

    return func;
}
