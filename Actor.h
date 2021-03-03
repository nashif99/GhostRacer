#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;
class Actor : public GraphObject
{
public:
	Actor(StudentWorld *w,int id, int sx, int sy, int direction, double size, int depth, double vspeed, double hspeed, bool
		alive, int hp, bool cawa);

	~Actor();


	virtual void doSomething() = 0;
	virtual void move();
	virtual bool beSprayed(); //whether or not hte object can be sprayed (is affected by spray)
	virtual void getSprayed(); 
	bool overlapsWithGR();
	virtual bool isSpray(); //only returns true if the object is holy water spray
	int getVertSpeed(); //get current actor's vertical speed
	void setVertSpeed(int vs); //set actor's vert speed to specified amount
	int getHorizSpeed(); //get current actor's horiz speed 
	void setHorizSpeed(int hsf); //set actor's horiz speed to specified amount
	bool Alive(); //check alive status
	void setAlive(bool alive); // set the alive status to true (alive) or false(dead)
	int getHealth(); //get health level
	void setHealth(int newhealth); //set health of objects (for damage)
	bool isCollisionAvoidance(); //returns collision avoidance worthy status
	void setGhostRacerSpeed(int racer); //set GR vert speed 
	int getGhostRacerSpeed(); // get GR vert speed, some actors require information of the Ghost Racer's speed
	StudentWorld* getWorld(); //return the student world object
	int getLane(); //return the lane the actor is in
	virtual bool isZC(); // is the actor a zombie car


private:
	double vertspeed;
	double horizspeed;
	bool isAlive;
	int health;
	bool collision_avoidance;
	int RacerVertSpeed;
	StudentWorld* world;

};
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class BorderLine : public Actor
{
public:
	BorderLine(StudentWorld* w, int x, int y, int id);
	~BorderLine();

	virtual void doSomething();






};





class GhostRacer : public Actor
{
public:
	GhostRacer(StudentWorld *w);
	~GhostRacer();
	virtual void doSomething();
	void move();
	int getSprays();
	void addSprays(int howMany);
	void spin();

private:
	int HolyWaterCount;
	StudentWorld* world;
};

class HolyWaterProjectile : public Actor
{
public:
	HolyWaterProjectile(StudentWorld* w, int sx, int sy, int dir, double vertspeed, double horizspeed);
	virtual void doSomething();
	virtual bool isSpray();

private:
	int maxTravelDist;
};



class Pedestrian : public Actor
{
public:
	Pedestrian(StudentWorld* w, int id, int sx, int sy, double size);
	//virtual void hit() = 0;
	void changeDirection(); //change direction
	//bool overlapsWithGR(); //check overlap with GR
	//void move();
	int getMovementPlanDist();
	void setMovementPlanDist(int dist);
	void newMovementPlan();
	virtual bool beSprayed();


private:
	int movementPlanDist;

};

class HumanPedestrian : public Pedestrian //derive HumanPed from Pedestrian class 
{
public:
	HumanPedestrian(StudentWorld* w, int sx, int sy);
	virtual void doSomething();
	virtual void getSprayed();
	
};

class ZombiePedestrian : public Pedestrian
{
public: 
	ZombiePedestrian(StudentWorld* w, int sx, int sy);
	virtual void doSomething();
	virtual void getSprayed();

private:
	int ticksToGrunt;
};


class ZombieCab : public Actor
{
public:
	ZombieCab(StudentWorld* w, int sx, int sy, double vertspeed);
	virtual void doSomething();
	virtual bool isZC();
	virtual bool beSprayed();
	virtual void getSprayed();
	
private:
	int planLength;
	bool damagedGR;
	bool cawaClose;
};


class Goodies:public Actor
{
public:
	Goodies(StudentWorld* w, int id, int sx, int sy, double size, int dir);
	//virtual bool beSprayed();
};

class SprayableGoodies :public Goodies //create class for goodies that can be sprayed
{
public:
	SprayableGoodies(StudentWorld* w, int id, int sx, int sy, double size,int dir);
	virtual bool beSprayed();
	virtual void getSprayed();
};
class OilSlick:public Goodies
{
public:
	OilSlick(StudentWorld* w, int sx, int sy);
	virtual void doSomething();
	//virtual bool beSprayed();
};

class HealingGoodie :public SprayableGoodies
{
public:
	HealingGoodie(StudentWorld* w, int sx, int sy);
	virtual void doSomething();
};

class WaterGoodie :public SprayableGoodies
{
public:
	WaterGoodie(StudentWorld* w, int sx, int sy);
	virtual void doSomething();
};
class SoulGoodie :public Goodies
{
public:
	SoulGoodie(StudentWorld* w, int sx, int sy);
	virtual void doSomething();

};



#endif // ACTOR_H_
