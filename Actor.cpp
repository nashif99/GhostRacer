
#define _USE_MATH_DEFINES
#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

/*###################################ACTOR BASE CLASS##########################################################*/
Actor::Actor(StudentWorld *w,int id, int sx, int sy, int direction, double size, int depth, double vspeed, double hspeed, bool
	alive, int hp, bool cawa) : GraphObject(id, sx, sy, direction, size, depth),world(w),
	vertspeed(vspeed), horizspeed(hspeed), isAlive(alive), health(hp), collision_avoidance(cawa) {

};

Actor::~Actor() {

};


int Actor::getVertSpeed() {
	return vertspeed;
};

void Actor::setVertSpeed(int vs) {
	vertspeed = vs;
}
int Actor::getHorizSpeed() {
	return horizspeed;
};

void Actor::setHorizSpeed(int hs) {
	horizspeed = hs;
}
bool Actor::Alive() {
	return isAlive;
};

void Actor::setAlive(bool alive) {
	isAlive = alive;
}

//by default, actors are not sprayable
bool Actor::beSprayed() {
	return false;

}

// getSprayed does nothing in the base class

void Actor::getSprayed() {

}

// find out if actor is holy water projectile, default false
bool Actor::isSpray() {
	return false;
}

int Actor::getHealth() {
	return health;
};
void Actor::setHealth(int newhealth) {
	if (newhealth <= 0) {
		setAlive(false);
		health = 0;
	}
	else
		health = newhealth;
}
bool Actor::isCollisionAvoidance() {
	return collision_avoidance;
};
void Actor::setGhostRacerSpeed(int racer) {
	RacerVertSpeed = racer;
}

int Actor::getGhostRacerSpeed() {
	return RacerVertSpeed;
}

StudentWorld* Actor::getWorld() {
	return world;
}

void Actor::move() {
	int vert_speed = getVertSpeed() - getGhostRacerSpeed();
	int horiz_speed = getHorizSpeed();
	double new_y = this->getY() + vert_speed;
	double new_x = this->getX() + horiz_speed;
	GraphObject::moveTo(new_x, new_y);
	if (this->getX() > VIEW_WIDTH || this->getY() > VIEW_HEIGHT || this->getX() < 0 || this->getY() < 0) {
		setAlive(false);
		return;
	}


}

bool Actor::overlapsWithGR() { //use StudentWorld overlaps and getGR functions to check overlap
	return getWorld()->overlaps(this, getWorld()->getGhostRacer());
};

int Actor::getLane() {
	int x = getX();
	// divide road into sixths, first lane is first two sixths, second lane is next to sixths, so on
	if (x < ROAD_CENTER - ROAD_WIDTH / 6 && x >= ROAD_CENTER - ROAD_WIDTH / 2) //left lane
		return 1;
	if (x >= ROAD_CENTER - ROAD_WIDTH / 6 && x < ROAD_CENTER + ROAD_WIDTH / 6) //middle lane
		return 2;
	if (x >= ROAD_CENTER + ROAD_WIDTH / 6 && x < ROAD_CENTER + ROAD_WIDTH / 2) // right lane
		return 3;
	else
		return 0;
}

bool Actor::isZC() {
	return false;
}
/*###################################BORDERLINE CLASS#######################################################*/




BorderLine::BorderLine(StudentWorld* w,int x, int y, int id) : Actor(w,id, x, y, 0, 2.0, 2, -4, 0, true, 0, false) {};

BorderLine::~BorderLine() {};

void BorderLine::doSomething() {
	move();
}

/*###################################GR CLASS#################################################################*/


GhostRacer::GhostRacer(StudentWorld *w) : Actor(w,IID_GHOST_RACER, 128, 32, 90, 4.0, 0, 0, 0, true, 100, true), HolyWaterCount(10),world(w) {

};
GhostRacer::~GhostRacer() { };

/*
StudentWorld* GhostRacer::getWorld() {
	return world;
}
*/
int GhostRacer::getSprays() {
	return HolyWaterCount;
}

void GhostRacer::addSprays(int howMany) {
	HolyWaterCount += howMany;
}
void GhostRacer::doSomething() {
	if(this->Alive() != true)
		return;
	if (this->getX() <= ROAD_CENTER-ROAD_WIDTH/2) {
		if (this->getDirection() > 90) {
			setHealth(getHealth() - 10);
		}
		setDirection(82);
		// PLAY SOUND //
		getWorld()->playSound(SOUND_VEHICLE_CRASH);
		move();
	}
	if (this->getX() >= ROAD_CENTER + ROAD_WIDTH / 2) {
		if (this->getDirection() < 90) {
			setHealth(getHealth() - 10);
		}
		setDirection(98);
		// PLAY SOUND //
		getWorld()->playSound(SOUND_VEHICLE_CRASH);
		move();
	}

	int ch;

	if (getWorld()->getKey(ch)) {

		switch (ch) {
		case KEY_PRESS_SPACE:
			if (HolyWaterCount >= 1) {
				double delta_x = cos(getDirection() * (M_PI / 180)) * SPRITE_HEIGHT;
				double delta_y = cos(getDirection() * (M_PI / 180)) * SPRITE_HEIGHT;
				double cur_x = getX();
				double cur_y = getY();

				world->addActor(new HolyWaterProjectile(world, cur_x + delta_x, cur_y + delta_y, getDirection(), 5, 5));
				getWorld()->playSound(SOUND_PLAYER_SPRAY);
				HolyWaterCount--;
			}
			break;

		case KEY_PRESS_LEFT:
			if (getDirection() < 114) {
				setDirection(getDirection() + 8);
				move();
			}

			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() > 66) {
				setDirection(getDirection() - 8);
				move();
			}
			break;
		case KEY_PRESS_UP:
			if (getVertSpeed() < 5) {
				setVertSpeed(getVertSpeed() + 1);
				move();
			}
			break;
		case KEY_PRESS_DOWN:
			if (getVertSpeed() > -1) {
				setVertSpeed(getVertSpeed() - 1);
				move();
			}
			break;
		default:
			break;




		}



	}
	move();
	return;
};

void GhostRacer::move() {
	double max_shift_per_tick = 4.0;
	int direction = this->getDirection();
	double delta_x = cos(direction*(M_PI/180) ) * max_shift_per_tick;
	double cur_x = getX();
	double cur_y = getY();
	moveTo(cur_x + delta_x, cur_y);
}

void GhostRacer::spin() {
	int randDirection = randInt(5, 20);
	int wise = randInt(1, 2); //pick clockwise or counterclockwise
	if (wise == 1) {
		if (getDirection() - randDirection < 60)
			setDirection(60);
		else
			setDirection(getDirection() - randDirection);
	}
	else {
		if (getDirection() + randDirection > 120)
			setDirection(120);
		else
			setDirection(getDirection() + randDirection);
	}
}

/*################################################## Holy Water Projectile ##########################*/

HolyWaterProjectile::HolyWaterProjectile(StudentWorld* w, int sx, int sy, int dir, double vertspeed, double horizspeed) : Actor(w, IID_HOLY_WATER_PROJECTILE, sx, sy, dir, 1.0, 1, vertspeed, horizspeed, true, 0, false), maxTravelDist(160)
{


};

bool HolyWaterProjectile:: isSpray() {
	return true;
}

void HolyWaterProjectile::doSomething() {
	if (!Alive())
		return;

	/* IMPLEMENT ACTIVATION*/

	moveForward(7);
	maxTravelDist = maxTravelDist - 7;
	if (getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT || maxTravelDist <= 0) {
		setAlive(false);
		return;
	}
};



/*#####################################################Pedestrian Class#####################################*/

Pedestrian::Pedestrian(StudentWorld* w, int id, int sx, int sy, double size) :Actor(w, id, sx, sy, 0, size, 0, -4, 0, true, 2, true),movementPlanDist(0) {

}

void Pedestrian::changeDirection() {
	if (getDirection() == 180) {
		setDirection(0);
	}
	else
		setDirection(180);
};




int Pedestrian::getMovementPlanDist() {
	return movementPlanDist;
};

void Pedestrian::setMovementPlanDist(int dist) {
	movementPlanDist = dist;
}
/*
void Pedestrian::move() {
	int vert_speed = getVertSpeed() - getGhostRacerSpeed();
	int horiz_speed = getHorizSpeed();
	double new_y = getY() + vert_speed;
	double new_x = getX() + horiz_speed;
	moveTo(new_x, new_y);
}
*/

void Pedestrian::newMovementPlan() {
	int random = randInt(-3, 3);
	if (random == 0) { //cant be zero, keep rolling until no longer zero
		while (random == 0)
			random = randInt(-3, 3);
	}
	setHorizSpeed(random);
	setMovementPlanDist(randInt(4, 32));
	if (random < 0)
		setDirection(180);
	else
		setDirection(0);
};

bool Pedestrian::beSprayed() {
	return true;
};

/*########################################################Human Pedestrian Class #################################################*/


HumanPedestrian::HumanPedestrian(StudentWorld* w, int sx, int sy): Pedestrian(w,IID_HUMAN_PED,sx,sy,2.0) {

}

void HumanPedestrian::getSprayed() {
	setHorizSpeed(getHorizSpeed() * -1);
	changeDirection();
	getWorld()->playSound(SOUND_PED_HURT);
}

void HumanPedestrian::doSomething() {
	if (!Alive()) {
		return;
	}
	if (overlapsWithGR()) {
		//getWorld()->decLives();
		getWorld()->getGhostRacer()->setAlive(false);
		getWorld()->playSound(SOUND_PLAYER_DIE);
		return;
	}
	move();

	setMovementPlanDist(getMovementPlanDist() - 1);
	if (getMovementPlanDist() > 0) {
		return;
	}
	newMovementPlan();



};


/*##############################################################Zombie Pedestrian###########################################################*/

ZombiePedestrian::ZombiePedestrian(StudentWorld* w, int sx, int sy) :Pedestrian(w, IID_ZOMBIE_PED, sx, sy, 3.0),ticksToGrunt(0) {
}

void ZombiePedestrian::doSomething() {
	if (!Alive())
		return;
	Actor* gr = getWorld()->getGhostRacer();
	if (overlapsWithGR()) {
		gr->setHealth(gr->getHealth() - 5);
		this->setHealth(this->getHealth() - 2);
		this->setAlive(false);
		getWorld()->playSound(SOUND_PED_DIE);
		getWorld()->increaseScore(150);
		return;
	}
	double left_range = gr->getX() - 30;
	double right_range = gr->getX() + 30;
	if (getX() > left_range && getX() < right_range && getY() > gr->getY()) {
		setDirection(270);
		if (getX() > left_range && getX() < gr->getX()) //zombie ped is to the left of GR
			setHorizSpeed(1);
		else if (getX() < right_range && getX() > gr->getX()) // zombie ped is to the right
			setHorizSpeed(-1);
		else
			setHorizSpeed(0);
		ticksToGrunt--;
		if (ticksToGrunt <= 0) {
			getWorld()->playSound(SOUND_ZOMBIE_ATTACK);
			ticksToGrunt = 20;
		}

	}
	move();
	if (getMovementPlanDist() > 0) {
		setMovementPlanDist(getMovementPlanDist() - 1);
		return;
	}
	newMovementPlan();
};


void ZombiePedestrian::getSprayed() {
	setHealth(getHealth() - 1);
	if (getHealth() <= 0) {
		if (!getWorld()->overlaps(this,getWorld()->getGhostRacer())) {
			////////// ADD HEALING GOODIE
			if (randInt(1, 5) == 1)
				getWorld()->addActor(new HealingGoodie(getWorld(), getX(), getY()));
		}
		this->setAlive(false);
		getWorld()->playSound(SOUND_PED_DIE);
		getWorld()->increaseScore(150);
	}
	else
		getWorld()->playSound(SOUND_PED_HURT);
		
}



/*######################################################ZombieCab Class#####################################*/

ZombieCab::ZombieCab(StudentWorld* w, int sx, int sy, double vertspeed) :Actor(w, IID_ZOMBIE_CAB, sx, sy, 90, 4.0, 0, vertspeed, 0, true, 3, true), planLength(0), damagedGR(false),cawaClose(false) {

}

void ZombieCab::doSomething() {
	if (!Alive())
		return;
	GhostRacer* gr = getWorld()->getGhostRacer();
	if (overlapsWithGR()) { //check overlap with gr and perform necessary function
		if (damagedGR)
			move();
		else {
			getWorld()->playSound(SOUND_VEHICLE_CRASH);
			gr->setHealth(gr->getHealth() - 20);
			if (getX() < gr->getX() || getX() == gr->getX()) {
				this->setHorizSpeed(-5);
				this->setDirection(120 + randInt(0, 19));
			}
			if (getX() > gr->getX()) {
				this->setHorizSpeed(5);
				this->setDirection(60-randInt(0, 19));

			}
			damagedGR = true;
			
		}
		
	}
	move();
	if (getVertSpeed() > gr->getVertSpeed()) { //use zombiecabhelper functions to perform the necessary actions depending on whether its speed >/< than GR
		getWorld()->ZombiecabHelper1(this);
		return;
	}
	else {
		getWorld()->ZombiecabHelper2(this);
		return;
	}
	if (planLength > 0)
		return;
	else {
		planLength = randInt(4, 32);
		setVertSpeed(getVertSpeed() + randInt(-2, 2));
	}
}

bool ZombieCab::isZC() {
	return true;
}

void ZombieCab::getSprayed() {
	setHealth(getHealth() - 1);
	if (!Alive()) {
		getWorld()->playSound(SOUND_VEHICLE_DIE);

		////// implement new oil slick
		if (randInt(1, 5) == 1)
			getWorld()->addActor(new OilSlick(getWorld(), getX(), getY()));

		getWorld()->increaseScore(200);
		return;
	}
	else
		getWorld()->playSound(SOUND_VEHICLE_HURT);
}

bool ZombieCab::beSprayed() {
	return true;
}



/*############################################Goodies Class################################################*/

Goodies::Goodies(StudentWorld* w, int id,int sx, int sy, double size, int dir):
	Actor(w,id,sx,sy,dir,size,2,-4,0,true,0,false)
{

}

/*
bool Goodies::beSprayed() {  //changed design, goodies not sprayable by default
	return true;
};
*/



/*#############################################Oil Slick Class#####################################################*/

OilSlick::OilSlick(StudentWorld* w, int sx, int sy) :
	Goodies(w, IID_OIL_SLICK, sx, sy, randInt(2, 5),0)
{

}

void OilSlick::doSomething() {
	move();
	GhostRacer* gr = getWorld()->getGhostRacer();
	if (getWorld()->overlaps(this, gr)) {
		getWorld()->playSound(SOUND_OIL_SLICK);
		gr->spin();
	}
}

/*
bool OilSlick::beSprayed() {
	return false;
}
*/
/*######################################################Sprayable Goodies Class###########################################*/

SprayableGoodies::SprayableGoodies(StudentWorld* w, int id, int sx, int sy, double size, int dir) :
	Goodies(w, id, sx, sy, size,dir)
{

};

bool SprayableGoodies::beSprayed() {  //changed design, goodies not sprayable by default
	return true;
};

void SprayableGoodies::getSprayed() {
	setAlive(false);
}


/*#################################################Healing Goodie###############################################*/

HealingGoodie::HealingGoodie(StudentWorld* w, int sx, int sy) :
	SprayableGoodies(w, IID_HEAL_GOODIE, sx, sy, 1.0,0)
{

};

void HealingGoodie::doSomething() {
	move();
	GhostRacer* gr = getWorld()->getGhostRacer();
	if (getWorld()->overlaps(this, gr)) {
		gr->setHealth(gr->getHealth() + 10);
		setAlive(false);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(250);
	}
}

/*##############################################################Water Goodie#############################################*/

WaterGoodie::WaterGoodie(StudentWorld *w,int sx,int sy):
	SprayableGoodies(w,IID_HOLY_WATER_GOODIE,sx,sy,2.0,90){
}

void WaterGoodie::doSomething() {
	move();
	GhostRacer* gr = getWorld()->getGhostRacer();
	StudentWorld* world = getWorld();
	if (world->overlaps(this, gr)) {
		gr->addSprays(10);
		setAlive(false);
		world->playSound(SOUND_GOT_GOODIE);
		world->increaseScore(50);
	}
}
/*###################################################Sould Goodie#############################################*/

SoulGoodie::SoulGoodie(StudentWorld* w, int sx, int sy) :
	Goodies(w, IID_SOUL_GOODIE, sx, sy, 4.0,0)
{

}

void SoulGoodie::doSomething() {
	move();
	GhostRacer* gr = getWorld()->getGhostRacer();
	if (getWorld()->overlaps(this, gr)) {
		getWorld()->decSouls2Save();
		setAlive(false);
		getWorld()->playSound(SOUND_GOT_SOUL);
		getWorld()->increaseScore(100);
	}
}
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
