#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath),r(nullptr)
{
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::init()
{
    r = new GhostRacer(this);  //create a new ghost racer pointed to by r

    for (int j = 0; j < N; j++) { //create initial road

        actors.push_back(new BorderLine(LEFT_EDGE, j*SPRITE_HEIGHT, IID_YELLOW_BORDER_LINE));
        actors.push_back(new BorderLine(RIGHT_EDGE, j * SPRITE_HEIGHT, IID_YELLOW_BORDER_LINE));
    }
    for (int j = 0; j < M; j++) {

        actors.push_back(new BorderLine(LEFT_EDGE+ROAD_WIDTH/3, j *(4* SPRITE_HEIGHT), IID_WHITE_BORDER_LINE));
        actors.push_back(new BorderLine(RIGHT_EDGE-ROAD_WIDTH/3, j *(4* SPRITE_HEIGHT), IID_WHITE_BORDER_LINE));
    }



    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    r->doSomething(); //have the ghost racer do something and check its alive statist
    if (r->Alive() != true) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    /*
    vector<Actor*>::iterator it;
    it = actors.begin();
    while (it != actors.end()) {
        (*it)->setGhostRacerSpeed(r);
        (*it)->doSomething();
        if ((*it)->Alive() != true) {
            delete (*it);
            //actors.erase(it);
            //break;
        }
        it++;
    }
    */
    for (int i = 0; i < actors.size(); i++) { //let every actor do soemthing
        if (actors[i]->Alive()) {
            actors[i]->setGhostRacerSpeed(r);
            actors[i]->doSomething();


        }
        
    }

    vector<Actor*>::iterator it;
    it = actors.begin();
    while (it != actors.end()) {
        if ((*it)->Alive() != true) {
            delete (*it);
            actors.erase(it);
            it = actors.begin();
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
        actors.push_back(new BorderLine(LEFT_EDGE, new_border_y, IID_YELLOW_BORDER_LINE));
        actors.push_back(new BorderLine(RIGHT_EDGE, new_border_y, IID_YELLOW_BORDER_LINE));
    }

    if (delta_y >= 4 * SPRITE_HEIGHT) {
        actors.push_back(new BorderLine(LEFT_EDGE + ROAD_WIDTH / 3, VIEW_HEIGHT, IID_WHITE_BORDER_LINE));
        actors.push_back(new BorderLine(RIGHT_EDGE - ROAD_WIDTH / 3, VIEW_HEIGHT, IID_WHITE_BORDER_LINE));
    }



    int count = 0;
    int ch;

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (int i = 0; i < actors.size(); i++) {
        delete actors[i];
    }
    actors.clear();
}
