#include <object.h>
#include <behavior.h>

Behavior::Behavior(behavior_function action): Component(Component::BEHAVIOR) {
    actions.push_back(action);
}

Behavior::Behavior(std::list<behavior_function> actions): Component(Component::BEHAVIOR), actions(actions) {

}

void Behavior::add(Object* obj, behavior_function action) {
    Behavior *behavior = (Behavior*)obj->getComponent(Component::BEHAVIOR);
    if (behavior == NULL) {
        behavior = new Behavior(action);
        obj->addComponent(behavior);
    } else {
        behavior->actions.push_back(action);
    }
}
