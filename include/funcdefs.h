#ifndef FUNCDEFS_H_INCLUDED
#define FUNCDEFS_H_INCLUDED

#include <utils.h>
#include <vec.h>
#include <transform.h>
#include <renderer.h>
#include <graphicssubsystem.h>
#include <collider.h>
#include <eventmanager.h>
#include <synchronizer.h>
#include <networksubsystem.h>
#include <scenemanager.h>
#include <camera.h>

#include <game.h>

#include "BitStream.h"

inline void rotateObject(Object &obj) {
    int degreesPerSecond = 90;
    Transform::rotateObj(&obj, vec3f(0, 0, degreesPerSecond*GraphicsSubsystem::delta));
}

inline void moveObject(Object &obj) {
    vec3f &moveVec = Game::get().connectedPlayers[obj.tag]->moveVec;
    Collider *c = Collider::get(obj);
    std::list<Collider::Collision>::iterator col = c->collisions().begin();
    vec3f newPos = Transform::get(obj)->position+moveVec*Game::get().localPlayer.movePerSecond*GraphicsSubsystem::delta;
    for (; col != c->collisions().end(); col++) {
        //alter movement vector so we cannot go beyond collision points
        //printf("collision normal %f %f %f \n", col->normal.x, col->normal.y, col->normal.z);
        Transform *otherCol = Transform::get(*col->with.owner());
        if (otherCol->position.x() > newPos.x() && newPos.x() >= col->point.x()) moveVec.x() += moveVec.x()*col->normal.x(); //x+
        else if (otherCol->position.x() < newPos.x() && newPos.x() <= col->point.x()) moveVec.x() -= moveVec.x()*col->normal.x(); //x-
        if (otherCol->position.z() > newPos.z() && newPos.z() >= col->point.z()) moveVec.z() += moveVec.z()*col->normal.z(); //z+
        else if (otherCol->position.z() < newPos.z() && newPos.z() <= col->point.z()) moveVec.z() -= moveVec.z()*col->normal.z(); //z-
    }
    Transform::translateObj(&obj, moveVec*Game::get().localPlayer.movePerSecond*GraphicsSubsystem::delta);
    //reset movement vector for next frame
    moveVec = vec3f(0);
}

inline void colorCollidingObjects(Object &obj) {
    Collider *c = Collider::get(obj);
    std::list<Collider::Collision>::iterator col = c->collisions().begin();
    for (; col != c->collisions().end(); col++) {
        //printf("colliding with %s\n", col->with.owner()->name.c_str());
        Renderer::get(*col->with.owner())->material.diffuse = vec3f(0,1,0);
    }
}

inline void resetColorIfNoCollisions(Object &obj) {
    Collider *c = Collider::get(obj);
    if (c->collisions().size() == 0) Renderer::get(obj)->material.diffuse = vec3f(1,0,0);
}

inline void movementSynchronizer(Object& obj, RakNet::BitStream &bs, bool write) {
    Transform *t = Transform::get(obj);
    if (t != NULL) {
        if (write) {
            if (!NetworkSubsystem::isServer) {
                //for checking whether we can move this object, could be a security token
                std::string addr = NetworkSubsystem::rpc->GetRakPeer()->GetMyBoundAddress().ToString();
                RakNet::RakString ident(obj.tag.c_str());
                bs << ident;
                if (addr.compare(obj.tag) == 0) {
                    vec3f &moveVec = Game::get().localPlayer.moveVec;
                    bs << moveVec;
                    moveVec = vec3f(0);
                }
            } else {
                bs << t->position;
            }
        } else {
            if (!NetworkSubsystem::isServer) {
                bs >> t->position;
            } else {
                RakNet::RakString ident;
                bs >> ident;
                if (obj.tag.compare(ident.C_String()) == 0) {
                    vec3f &moveVec = Game::get().connectedPlayers[obj.tag]->moveVec;
                    bs >> moveVec;
                }
            }
        }
    }
}

inline void OnPlayerConnected(const char* addr) {
    NetworkSubsystem::rpc->SetRecipientAddress(RakNet::SystemAddress(addr), false);
    std::string pName = std::string("player-")+addr;
    Object *pc = Game::Player::createCharacter(pName, addr);

    //instantiate dynamic scene objects on the new remote client (currently other 'players')
    std::map<std::string,Game::Player*>::iterator p = Game::get().connectedPlayers.begin();
    for (; p != Game::get().connectedPlayers.end(); p++) {
        Object *pc1 = Object::Find(p->second->name);
        if (pc != NULL) {
            NetworkSubsystem::rpc->CallC("RemoteInstantiate", *pc1);
        }
    }

    NetworkSubsystem::rpc->SetRecipientAddress(NetworkSubsystem::rpc->GetRakPeer()->GetMyBoundAddress(), true);
    Game::get().connectedPlayers.insert(std::pair<std::string,Game::Player*>(std::string(addr), new Game::Player(pName)));
    Instantiate(*pc);
    printf("broadcasting instantiate for '%s'\n", pName.c_str());
    NetworkSubsystem::rpc->CallC("RemoteInstantiate", *pc);

}

inline void OnPlayerDisconnected(const char* addr) {
    NetworkSubsystem::rpc->SetRecipientAddress(NetworkSubsystem::rpc->GetRakPeer()->GetMyBoundAddress(), true);
    std::string pName = std::string("player-")+addr;
    Object *pc = Object::Find(pName);

    printf("broadcasting destroy for '%s'\n", pc->name.c_str());
    RakNet::RakString rs(pc->name.c_str());
    NetworkSubsystem::rpc->CallC("RemoteDestroy", rs);
    Destroy(*pc);
    Game::get().connectedPlayers.erase(std::string(addr));
}

#endif
