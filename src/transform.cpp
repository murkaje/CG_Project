
#include <transform.h>

Transform::Transform(vector3f position, vector3f rotation, vector3f scale):
Component(Component::TRANSFORM), position(position), rotation(rotation), scale(scale) {

}
