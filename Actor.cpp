
#define _USE_MATH_DEFINES
#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>


Actor::Actor(int id, int sx, int sy, int direction, double size, int depth, int vspeed, int hspeed, bool
	alive, int hp, bool cawa) : GraphObject(id, sx, sy, direction, size, depth),
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

bool Actor::Alive() {
	return isAlive;
};

void Actor::setAlive(bool alive) {
	isAlive = alive;
}
int Actor::getHealth() {
	return health;
};
void Actor::setHealth(int newhealth) {
	if (health - newhealth <= 0) {
		setAlive(false);
		health = 0;
	}
	else
		health = newhealth;
}
bool Actor::isCollisionAvoidance() {
	return collision_avoidance;
};
void Actor::setGhostRacerSpeed(const Actor* racer) {
	RacerVertSpeed = racer->vertspeed;
}

int Actor::getGhostRacerSpeed() {
	return RacerVertSpeed;
}
BorderLine::BorderLine(int x, int y, int id) : Actor(id, x, y, 0, 2.0, 2, -4, 0, true, 0, false) {};

BorderLine::~BorderLine() {};

void BorderLine::doSomething() {
	int vert_speed = getVertSpeed() - getGhostRacerSpeed();
	int horiz_speed = getHorizSpeed();
	double new_y = this->getY() + vert_speed;
	double new_x = this->getX() + horiz_speed;
	GraphObject::moveTo(new_x, new_y);
	if (this->getX() > VIEW_WIDTH || this->getY() > VIEW_HEIGHT || this->getX() < 0 || this-> getY() < 0) {
		setAlive(false);
		return;
	}


}



GhostRacer::GhostRacer(GameWorld *w) : Actor(IID_GHOST_RACER, 128, 32, 90, 4.0, 0, 0, 0, true, 100, true), HolyWaterCount(10),world(w) {

};
GhostRacer::~GhostRacer() { };


GameWorld* GhostRacer::getWorld() {
	return world;
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
		if (this->getDirection() > 90) {
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

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
