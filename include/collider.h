#ifndef COLLIDER_H
#define COLLIDER_H
#include<object.h>


class Collider
{
    public:
        Collider();
        void checkIntersection(Object *obj);
        virtual ~Collider();
    protected:
    private:
};

#endif // COLLIDER_H
