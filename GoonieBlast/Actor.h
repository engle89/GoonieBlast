#ifndef ACTOR_H_
#define ACTOR_H_
#include < cmath > 
#include "GraphObject.h"

using std::vector;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

//actor class
class Actor:public GraphObject
{
public:
	Actor(StudentWorld* p, int picID, int posX, int posY, Direction dir, int currentSub)
		:GraphObject(picID, posX, posY, dir, currentSub),m_life(true),m_ptr(p){};
	StudentWorld* getWorld();
	bool alive();
	void setDead();
	Actor* returnMyself();
	bool ckeckObstacle(int x, int y);
	virtual void doSomething();
	virtual void Attacked();
	virtual ~Actor() = 0;
private:
	StudentWorld* m_ptr;
	bool m_life;
};

//player class

class Player :public Actor
{
public:
	Player(StudentWorld* pp,int posX, int posY, int currentSub)
		:Actor(pp,0, posX, posY, right, currentSub), m_hitPoints(20), m_ammunition(0)
	{
		setVisible(true);
	};

	void resetHitPoints();
	void decreaseHitPoints(int howmuch);
	void increaseAmmunition(int number);
	int getHealth();
	int getHitPoints();
	int getAmmo();
	void assignHit(int newH);
	void assignAmmu(int newA);
	virtual void doSomething();
	virtual void Attacked();
	virtual ~Player(){};
private:
	int m_hitPoints;
	int m_ammunition;
};

//Wall class
class Wall :public Actor
{
public:
	Wall(StudentWorld* pp,int pos_X, int pos_Y, int current_Sub) :Actor(pp,6, pos_X, pos_Y, none, current_Sub){ setVisible(true); };
	virtual void doSomething();
	virtual void Attacked();

	virtual ~Wall(){};
};

//Bullet class
class Bullet :public Actor
{
public:
	Bullet(StudentWorld* pp,int posX, int posY, Direction dirbullet, int currentSub) 
		:Actor(pp,5, posX, posY, dirbullet, currentSub)
	{ setVisible(true);};

	virtual void doSomething();
	virtual void Attacked();

	virtual ~Bullet(){};

};

//Exit class
class Exit :public Actor
{
public:
	Exit(StudentWorld* p,int posX, int posY, int currentSub = 0) 
		:Actor(p,7, posX, posY, none, currentSub)
	{ setVisible(false);
	visibility = false; };

	void Revealed();
	bool returnvisibility();
	virtual void doSomething();
	virtual void Attacked();
	virtual ~Exit(){};
private:
	bool visibility;
};

//Goodie class and its derived classes 
class Goodie :public Actor
{
public:
	Goodie(StudentWorld* p,int PicID, int posX, int posY, int currentSub) 
		:Actor(p,PicID, posX, posY, none, currentSub)
	{setVisible(true); };
	
	virtual void doSomething();
	virtual void Attacked();
	virtual ~Goodie(){};
};

class ExtraLifeGoodie :public Goodie
{
public:
	ExtraLifeGoodie(StudentWorld* pp,int pos_X,int pos_Y,int current_Sub):Goodie(pp,11,pos_X,pos_Y,current_Sub){};
	virtual void doSomething();
	virtual ~ExtraLifeGoodie(){};
};

class RestoreHealthGoodie :public Goodie
{
public:
	RestoreHealthGoodie(StudentWorld* pp,int pos_X,int pos_Y,int current_Sub) :Goodie(pp,10,pos_X,pos_Y,current_Sub){};
	virtual void doSomething();
	virtual ~RestoreHealthGoodie(){};
};

class AmmoGoodie :public Goodie
{
public:
	AmmoGoodie(StudentWorld* pp,int pos_X,int pos_Y,int current_Sub) :Goodie(pp,12,pos_X,pos_Y,current_Sub){};
	virtual void doSomething();
	virtual ~AmmoGoodie(){};
};

class Hostage :public Goodie
{
public:
	Hostage(StudentWorld* pp,int pos_X, int pos_Y, int current_Sub) :Goodie(pp,3, pos_X, pos_Y, current_Sub){};
	virtual void doSomething();
	virtual ~Hostage(){};
};

class Jewel :public Goodie
{
public:
	Jewel(StudentWorld* pp,int pos_X, int pos_Y, int current_Sub) :Goodie(pp,9, pos_X, pos_Y, current_Sub){};
	virtual void doSomething();
	virtual ~Jewel(){};
};

class Gate :public Goodie
{
public:
	Gate(StudentWorld* pp,int pos_X, int pos_Y, int current_Sub,int recordSub) 
		:Goodie(pp,8, pos_X, pos_Y, current_Sub),sublevelTransportTo(recordSub){};

	virtual void doSomething();
	virtual ~Gate(){};
private:
	int sublevelTransportTo;
};

//Robot and its derived classes
class Robot :public Actor
{
public:
	Robot(StudentWorld* p,int PicId, Direction dirc,int hitpoints,int posX, int posY, int currentSub)
		:Actor(p, PicId, posX, posY, dirc, currentSub), m_hitPoints(hitpoints), tickCount(0), rest(true)
	{ setVisible(true);
	};
	int getHealth();
	int computrFrequency();
	Robot* returnRobot();
	bool returnRest();
	virtual void doSomething();
	virtual void damaged();
	virtual ~Robot(){};

private:
	int m_hitPoints;
	int tickCount;
	bool rest;
};

class SnarlBotHorizontal :public Robot
{
public:
	SnarlBotHorizontal(StudentWorld* pp,int pos_X,int pos_Y,int current_sub) 
		:Robot(pp,1, right,15,pos_X,pos_Y,current_sub){};

	virtual void doSomething();
	virtual void damaged();
	virtual ~SnarlBotHorizontal(){};
private:
	vector<Bullet*> m_bullets;

};

class SnarlBotVertical :public Robot
{
public:
	SnarlBotVertical(StudentWorld* pp,int pos_X, int pos_Y, int current_sub) 
		:Robot(pp,1, down,15,pos_X, pos_Y, current_sub){};

	virtual void doSomething();
	virtual void damaged();
	virtual ~SnarlBotVertical(){};
private:
	vector<Bullet*> m_bullets;
};

class KleptoBot :public Robot
{
public:
	KleptoBot(StudentWorld* pp,int pos_X, int pos_Y, int current_sub) 
		:Robot(pp, 2, right, 9, pos_X, pos_Y, current_sub), pickGoodieAlready(false), count(0)
	{
		switch (rand() % 3) {
		case 0: distanceBeforeTurning = 1; break;
		case 1: distanceBeforeTurning = 2; break;
		case 2:distanceBeforeTurning = 3; break;
		}
	};

	bool pickGoodieChance();
	virtual void doSomething();
	virtual void damaged();
	virtual ~KleptoBot(){};
private:
	int distanceBeforeTurning;
	int count;
	bool pickGoodieAlready;
	Goodie* m_treasure;
};

//KleptoBot Factory class
class Factory :public Actor
{
public:
	Factory(StudentWorld* p,int pos_X, int pos_Y, int current_sub)  // when it is constructed, create three kleptobots
		:Actor(p,4, pos_X, pos_Y, none, current_sub)
	{ setVisible(true); };

	virtual void doSomething();
	virtual void Attacked();
	virtual ~Factory(){};
};

#endif // ACTOR_H_
