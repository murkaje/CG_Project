
#include <behavior.h>

Behavior::Behavior(behavior_function action): Component(Component::BEHAVIOR) {
    actions.push_back(action);
}

Behavior::Behavior(std::list<behavior_function> actions): Component(Component::BEHAVIOR), actions(actions) {

}

