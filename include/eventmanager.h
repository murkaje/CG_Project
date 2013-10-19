#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <queue>
#include <map>
#include <string>

typedef void (*func_ptr)();

class EventManager {
    static std::queue<void (*)(void)> eventQueue;
    static std::map<std::string, void(*)()> cmdMap;

public:
    static void QueEvent(void (*func)(void));
    static void ParseEvents();
    static void RegisterCommand(const std::string &name, void (*func)());
    static func_ptr GetCommand(const std::string &name);
};
#endif // EVENTMANAGER_H
