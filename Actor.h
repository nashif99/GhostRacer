#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"
class Actor : public GraphObject
{
public:
	Actor(int id, int sx, int sy, int direction, double size, int depth, int vspeed, int hspeed, bool
		alive, int hp, bool cawa);

	~Actor();


	virtual void doSomething() = 0;

	int getVertSpeed(); //get current actor's vertical speed
	void setVertSpeed(int vs);
	int getHorizSpeed(); //get current actor's horiz speed 
	bool Alive(); //check alive status
	void setAlive(bool alive);
	int getHealth(); //get health level
	void setHealth(int newhealth); //set health of objects (for damage)
	bool isCollisionAvoidance(); //check for collision avoidance
	void setGhostRacerSpeed(const Actor* racer); //set GR vert speed 
	int getGhostRacerSpeed(); // get GR vert speed, some actors require information of the Ghost Racer's speed 


private:
	int vertspeed;
	int horizspeed;
	bool isAlive;
	int health;
	bool collision_avoidance;
	int RacerVertSpeed;

};
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class BorderLine : public Actor
{
public:
	BorderLine(int x, int y, int id);
	~BorderLine();

	virtual void doSomething();






};





class GhostRacer : public Actor
{
public:
	GhostRacer(GameWorld *w);
	~GhostRacer();
	virtual void doSomething();
	void move();
	GameWorld* getWorld();

private:
	int HolyWaterCount;
	GameWorld* world;
};
#endif // ACTOR_H_
