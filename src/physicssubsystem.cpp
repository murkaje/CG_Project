#include "physicssubsystem.h"
#include <scenemanager.h>
#include <vector3f.h>
#include <math.h>
#include <iostream>

using namespace std;

PhysicsSubsystem::PhysicsSubsystem()
{
    //ctor
}

PhysicsSubsystem::~PhysicsSubsystem()
{
    //dtor
}

void PhysicsSubsystem::PerformPhysicsChecks()
{
    Scene* sceneObj = &SceneManager::CurrentScene();
    std::list<Object> &objects = sceneObj->getObjsList();
    for (std::list<Object>::iterator iterObj = objects.begin(); iterObj != objects.end(); iterObj++)
    {
        checkIntersection(*iterObj);
    }
}

void PhysicsSubsystem::checkIntersection(Object &obj)
{
    Transform &objT = *Transform::get(obj);
    Scene* sceneObj = obj.getCurrentScene();
    std::list<Object> &objects = sceneObj->getObjsList();
    for (std::list<Object>::iterator iterObj = objects.begin(); iterObj != objects.end(); iterObj++)
    {
        Transform &iterObjT = *Transform::get(*iterObj);

        if(iterObj->name != obj.name && (*iterObj).name == "cube")
        {
//            float euclideanDistanceBetweenObjects = sqrt(pow((objT.position.x - iterObjT.position.x),2) +
//                                                    pow((objT.position.y - iterObjT.position.y),2) +
//                                                    pow((objT.position.z - iterObjT.position.z),2));

            float xDistance = fabs(objT.position.x - iterObjT.position.x);
            float yDistance = fabs(objT.position.y - iterObjT.position.y);
            float zDistance = fabs(objT.position.z - iterObjT.position.z);

            float xMinDistance = (objT.scale.x + iterObjT.scale.x)/float(2);
            float yMinDistance = (objT.scale.y + iterObjT.scale.y)/float(2);
            float zMinDistance = (objT.scale.z + iterObjT.scale.z)/float(2);

//                cout<<xDistance<<" x min: "<<xMinDistance<<endl;
//                cout<<yDistance<<" y min: "<<yMinDistance<<endl;
//                cout<<zDistance<<" z min: "<<zMinDistance<<endl;
            if(xDistance < xMinDistance && yDistance < yMinDistance && zDistance < zMinDistance)
            {
                Renderer::get((*iterObj))->material.diffuse = v3f(1,0,0);
//                cout<<"Colliding with "<<iterObj->name<<" !!!"<<endl;
            } else {
                Renderer::get((*iterObj))->material.diffuse = v3f(0,0,1);
            }
        }
    }
}
