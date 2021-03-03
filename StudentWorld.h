#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include "GameWorld.h"
#include "Actor.h"
#include <string>

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    GhostRacer* getGhostRacer();
    void addActor(Actor* a);
    bool overlaps(const Actor* a1, const Actor* a2) const;
    int returnPlayerDied() const;
    //bool sprayActor(Actor* a); //call the getSprayed actor function

    // functions to help zombie cab detect collision avoidance actors in lanes
    void ZombiecabHelper1(Actor *z);
    void ZombiecabHelper2(Actor* z);

    void addZombieCab();
    void decSouls2Save();

private:
    vector<Actor*> actors;
    GhostRacer* r;
    int bonus;
    int souls2save;
    int N = VIEW_HEIGHT / SPRITE_HEIGHT;
    int M = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;

};

#endif