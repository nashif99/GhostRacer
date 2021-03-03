#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath),r(nullptr),bonus(5000)
{
    
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

GhostRacer* StudentWorld::getGhostRacer() {
    return r;
}



//Zombie cab helpers loop through the actors to get their lanes, check collision avoidance, and move ZC appropriately
void StudentWorld::ZombiecabHelper1(Actor* z) {
    int cablane = z->getLane();
    int zy = z->getY();
    double close = zy + 96;
    if (r->getLane() == cablane) {
        if (r->getY() < close) {
            z->setVertSpeed(z->getVertSpeed() - .5);
            return;
        }
    }
    for (int i = 0; i < actors.size(); i++) {
        if (actors[i] != z) {
            int lane = actors[i]->getLane();
            if (lane == cablane && actors[i]->isCollisionAvoidance()) {
                if (actors[i]->getY() < close && actors[i]->getY() > zy) {
                    z->setVertSpeed(z->getVertSpeed() - .5);
                }
            }
        }
    }
}

void StudentWorld::ZombiecabHelper2(Actor* z) {
    int cablane = z->getLane();
    int zy = z->getY();
    double close = zy - 96;
    for (int i = 0; i < actors.size(); i++) {
        if (actors[i] != z) {
            int lane = actors[i]->getLane();
            if (lane == cablane && actors[i]->isCollisionAvoidance()) {
                if (actors[i]->getY() > close && actors[i]->getY() < zy) {
                    z->setVertSpeed(z->getVertSpeed() + .5);
                }
            }
        }
    }
}

void StudentWorld::addZombieCab() {
    bool lane_traversed[3] = { false,false,false };
    bool alltraversed = false;
    int lane_to_add = 0;
    double starty = 0;
    double initvspeed = 0;
    while (!alltraversed) { //traverse all possible lanes
        int cur_lane = randInt(1, 3);
        if (!lane_traversed[cur_lane - 1]) {
            double min = VIEW_HEIGHT;
            double max = 0;
            bool actorFound = false;
            for (int i = 0; i < actors.size(); i++) { //loop through all actors, checking the min and max of each lane
                if (r->getLane() == cur_lane) {
                    actorFound = true;
                    min = 0;//if the lane has ghost racer, skip check (gr is collision avoidance worthy)
                    break;
                }
                if (actors[i]->isCollisionAvoidance()) {
                    if (actors[i]->getLane() == cur_lane && r->getLane() == cur_lane) { //check if GR is also in the lane and return the closest one
                        actorFound = true;
                        if (min > std::min(actors[i]->getY(), r->getY()))
                            min = std::min(actors[i]->getY(), r->getY());
                        if (max < std::max(actors[i]->getY(), r->getY()))
                            max = std::max(actors[i]->getY(), r->getY());
                    }
                    else if (actors[i]->getLane() == cur_lane) {
                        actorFound = true;
                        if (min > actors[i]->getY())
                            min = actors[i]->getY();
                        if (max < actors[i]->getY())
                            max = actors[i]->getY();

                    }
                    else if (r->getLane() == cur_lane) {
                        actorFound = true;
                        if (min > r->getY())
                            min = r->getY();
                        if (max < r->getY())
                            max = r->getY();
                    }
                }
            }
            //check if the actor is found or if it meets the min or max requirements
            if (!actorFound || (actorFound && min > (VIEW_HEIGHT / 3))) {
                lane_to_add = cur_lane;
                starty = SPRITE_HEIGHT / 2;
                initvspeed = r->getVertSpeed() + randInt(2, 4);
                break;
            }
            if (!actorFound || (actorFound && max < (VIEW_HEIGHT * 2 / 3))) {
                lane_to_add = cur_lane;
                starty = VIEW_HEIGHT - SPRITE_HEIGHT / 2;
                initvspeed = r->getVertSpeed() - randInt(2, 4);
                break;
            }
            lane_traversed[cur_lane - 1] = true; //otherwise, this lane is traversed
            for (int j = 0; j < 3; j++) {
                if (lane_traversed[j] == false) //check for traversake if all lanes
                    break;
                if (j == 2)
                    alltraversed = true;
            }

        }
    }
    if (lane_to_add == 0)
        return;
    int startx;
    if (lane_to_add == 1) //left lane
        startx = ROAD_CENTER - ROAD_WIDTH / 3;
    else if (lane_to_add == 2) //mid lane
        startx = ROAD_CENTER;
    else
        startx = ROAD_CENTER + ROAD_WIDTH / 3;
    actors.push_back(new ZombieCab(this, startx, starty, initvspeed));

}

void StudentWorld::decSouls2Save() {
    souls2save--;
}


int StudentWorld::init()
{
    r = new GhostRacer(this);  //create a new ghost racer pointed to by r
    bonus = 5000;
    souls2save = 2 * getLevel() + 5;
    for (int j = 0; j < N; j++) { //create initial road with white and yellow borderlines

        actors.push_back(new BorderLine(this,LEFT_EDGE, j*SPRITE_HEIGHT, IID_YELLOW_BORDER_LINE));
        actors.push_back(new BorderLine(this,RIGHT_EDGE, j * SPRITE_HEIGHT, IID_YELLOW_BORDER_LINE));
    }
    for (int j = 0; j < M; j++) {

        actors.push_back(new BorderLine(this,LEFT_EDGE+ROAD_WIDTH/3, j *(4* SPRITE_HEIGHT), IID_WHITE_BORDER_LINE));
        actors.push_back(new BorderLine(this,RIGHT_EDGE-ROAD_WIDTH/3, j *(4* SPRITE_HEIGHT), IID_WHITE_BORDER_LINE));
    }



    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    r->doSomething(); //have the ghost racer do something and check its alive statist
    if (!r->Alive()) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    for (int i = 0; i < actors.size(); i++) { //let every actor do soemthing
        if (actors[i]->Alive()) {
            actors[i]->setGhostRacerSpeed(r->getVertSpeed());
            actors[i]->doSomething();
            if (actors[i]->isSpray()) { // checks if actors can be sprayed and sprays them
                for (int j = 0; j < actors.size(); j++) {
                    if (!actors[j]->isSpray()) {
                        if (overlaps(actors[i], actors[j]) && actors[j]->beSprayed()) {
                            actors[j]->getSprayed();
                            actors[i]->setAlive(false);
                        }
                    }
                }
            }

        }
        
    }

    vector<Actor*>::iterator it;
    it = actors.begin();
    while (it != actors.end()) { //iterate through vector deleting dead actors 
        if ((*it)->Alive() != true) {
            delete (*it);
         
            it = actors.erase(it);
        }
        else
            it++;
    }

    int new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    it = actors.end();
    int last_white_border_y;
    while (it != actors.begin()) { // find the last white border added to the vector
        it--;
        if ((*it)->getVertSpeed() == -4 && (*it)->getHorizSpeed() == 0 && (*it)->getHealth() == 0 && ((*it)->getX() == LEFT_EDGE+ROAD_WIDTH/3 || (*it)->getX() == RIGHT_EDGE-ROAD_WIDTH/3)) { //this combination of values should be unique to white borders
            last_white_border_y = (*it)->getY();
            break;
        }
    }

    int delta_y = new_border_y - last_white_border_y;
    if (delta_y >= SPRITE_HEIGHT) {
        actors.push_back(new BorderLine(this,LEFT_EDGE, new_border_y, IID_YELLOW_BORDER_LINE));
        actors.push_back(new BorderLine(this,RIGHT_EDGE, new_border_y, IID_YELLOW_BORDER_LINE));
    }

    if (delta_y >= 4 * SPRITE_HEIGHT) {
        actors.push_back(new BorderLine(this,LEFT_EDGE + ROAD_WIDTH / 3, VIEW_HEIGHT, IID_WHITE_BORDER_LINE));
        actors.push_back(new BorderLine(this,RIGHT_EDGE - ROAD_WIDTH / 3, VIEW_HEIGHT, IID_WHITE_BORDER_LINE));
    }
    
    int L = getLevel();
    int ChanceHumanPed = max(200 - L * 10, 30); //add human pedestrian
    int randomInt = randInt(0, ChanceHumanPed - 1);
    if (randomInt == 0) {
        int randomx = randInt(1, VIEW_WIDTH - 1);
        actors.push_back(new HumanPedestrian(this, randomx, VIEW_HEIGHT));
        //actors.push_back(new HumanPedestrian(this, 120, VIEW_HEIGHT));
    }

    int ChanceZombiePed = max(100 - L * 10, 20);
    randomInt = randInt(0, ChanceZombiePed - 1);
    if (randomInt == 0) {
        int randomx = randInt(1, VIEW_WIDTH - 1);
        actors.push_back(new ZombiePedestrian(this, randomx, VIEW_HEIGHT));
        //actors.push_back(new HumanPedestrian(this, 120, VIEW_HEIGHT));
    }

    int ChanceVehicle = max(100 - L * 10, 20);
    if (randInt(0, ChanceVehicle-1)==0)
        addZombieCab();

    int ChanceOilSlick = max(150 - L * 10, 40);
    if (randInt(0, ChanceOilSlick - 1) == 0)
        actors.push_back(new OilSlick(this, randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));

    if (randInt(0, 99) == 0)//chance of lost soul
        actors.push_back(new SoulGoodie(this, randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));
    if (randInt(0, 99 + 10 * L) == 0)//chance of holy water goodie
        actors.push_back(new WaterGoodie(this, randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));

    int count = 0;
    int ch;
    ostringstream oss;
    oss << setw(7 + to_string(getScore()).size()) << "Score: " << getScore();
    oss << setw(6 + to_string(getLevel()).size()) << "Lvl: " << getLevel();
    oss << setw(13 + to_string(souls2save).size()) << "Souls2Save: " << souls2save;
    oss << setw(8 + to_string(getLives()).size()) << "Lives: " << getLives();
    oss << setw(7 + to_string(r->getHealth()).size()) << "Health: " << r->getHealth();
    oss << setw(8 + to_string(r->getSprays()).size()) << "Sprays: " << r->getSprays();
    oss << setw(5 + to_string(bonus).size()) << "Bonus: " << bonus << endl;
    string s = oss.str();
    setGameStatText(s);
    if(bonus>0)
        bonus--;

    if (souls2save <= 0)
        return GWSTATUS_FINISHED_LEVEL;
    return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::addActor(Actor* a) {
    actors.push_back(a);
}

bool StudentWorld::overlaps(const Actor* a1, const Actor* a2) const {
    double a1x = a1->getX();
    double a2x = a2->getX();
    double a1y = a1->getY();
    double a2y = a2->getY();
    double delta_x = abs(a1x - a2x);
    double delta_y = abs(a1y - a2y);
    double rad_sum = a1->getRadius() + a2->getRadius();
    if (delta_x < rad_sum*.25 && delta_y < rad_sum*.6) 
        return true;
    return false;
}

int StudentWorld::returnPlayerDied() const {
    return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
    for (int i = 0; i < actors.size(); i++) { //go through vector of actor pointers and delete each
        delete actors[i];
    }
    actors.clear();
    delete r; //delete ghostracer
}
