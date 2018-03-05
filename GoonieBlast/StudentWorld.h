#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>


using std::vector;
using std::string;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Level;
class GraphObject;
class Actor;
class Player;
class Wall;
class Bullet;
class Exit;
class Goodie;
class ExtraLifeGoodie;
class RestoreHealthGoodie;
class AmmoGoodie;
class Hostage;
class Jewel;
class Gate;
class Robot;
class SnarlBotHorizontal;
class SnarlBotVertical;
class KleptoBot;
class Factory;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);

	//init function
	//called when game starts, player finshes and advances to next level, player loses life and restart
	//load current main  level maze from a data file
	// constructing a representation of the current main level in  your StudentWorld object
	// loading all of the main level¡¯s sub-levels
	//representation of all the sub-levels in your StudentWorld object
	virtual int init();

	//move function
	// automatically be called once during each tick
	//ask each of the game actors to do something 
	//dispose of actors
	virtual int move();

	void updateDisplayText();
	string setString(int score,int level,int sublevel,int lives,int health,int ammo,int bonus);

	int getCurrentTimeLimit();
	

	//clean up funtion
	//be called when player finishes current main level or dies
	//free all actors that are currently in the game
	virtual void cleanUp();

	~StudentWorld();
    

	//getCurrentSubLevel function
	//locate current sublevel
	virtual int getCurrentSubLevel();

	//get pointer of player
	Player* getPlayer();

	//get an actor ID
	 Actor* getActorAtLocation(int x, int y);

	 //get a robot pointer
	 Robot* getRobot(int x, int y);

	//manage the goodie picking up issue
	void pickGoodie(Actor* ap);

	//change sublevel
	void changeSublevel(int i);

	//add a Goodie
	void addGoodie(Actor* ap);

	//add kleptobots
	void addKleptoBots(Factory* ap);

	//add bullets
	void addBullets(int x, int y, Actor* ap);

	//add a player
	void addPlayer(int Sub);

	//count kleptobot
	int countKlpto(int x, int y);

	//inform whether finish this level
	bool advanceLevel();
	

private:
	Player* m_player[5];

	vector<Level*> m_level;

	vector<Bullet*> m_bullet;
	vector<vector<Wall*> > m_wall; 

	Exit* m_exit;

	vector<vector<ExtraLifeGoodie*> > m_extralife;
	vector<vector<RestoreHealthGoodie*> > m_restorehealth;
	vector<vector<AmmoGoodie*> > m_ammo;
	vector<vector<Hostage*> > m_hostages;
	vector<vector<Jewel*> > m_Jewel;
	vector<vector<Gate*> > m_gate;
	vector<vector<SnarlBotHorizontal*> > m_snarlh;
	vector<vector<SnarlBotVertical*> > m_snarlv;
	vector<vector<Factory*> > m_factory;
	vector<vector<KleptoBot*> > m_kleptobots;

	int timeLimit;
	int Tick;
	int count;
	int currentSubLevel;

	int numHostages;
	int numJewel;
};

#endif // STUDENTWORLD_H_
