// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
#include "StudentWorld.h"
#include <string>
#include <vector>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw

#include "Level.h"
#include "Actor.h"

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

//constructor
StudentWorld::StudentWorld(std::string assetDir):GameWorld(assetDir)
{
	timeLimit = 75;
	Tick = 1500;
	count = 20;
	numHostages = 0;
	numJewel = 0;
}

//1. Initialize the data structures used to keep track of your game¡¯s world.  
//2. Load the current main maze¡¯s and all its sub-mazes¡¯ details from level data file(s). 
//3. Allocate and insert a valid Player object into the game world. 
//4. Allocate and insert any SnarlBots objects, Wall objects, Hostage objects, Factory 
//objects, Jewel objects, Goodie objects, or Exit objects into the game world, as 
//required by the specification in the level¡¯s data file.

int StudentWorld::init()
{
	//Initialize the data structures used to keep track of your game¡¯s world.
	timeLimit = 75;
	Tick = 75 * 20;
	count = 20;
	currentSubLevel = 0;
	



	//load from level data file
	int x = getLevel();
	string filename;
	ostringstream oss;
	oss.fill('0');
	oss << "level" << setw(2) << x << ".dat";
	filename = oss.str();

	Level lev(assetDirectory());
	Level::LoadResult result =lev.loadLevel(filename);
	if (result == Level::load_fail_file_not_found || result == Level::load_fail_bad_format)
		return -1;
	m_level.push_back(new Level(assetDirectory()));
	m_level[x]->loadLevel(filename, 0);

	for (int k = 1; k <= 5; k++)
	{
		string filenameSub;
		ostringstream oss2;
		oss2.fill('0');
		oss2 << "level" << setw(2) << getLevel();
		oss2 << '_' << k << ".dat";
		filenameSub = oss2.str();
		Level lev(assetDirectory());
		Level::LoadResult result = lev.loadLevel(filenameSub,k);
		if (result == Level::load_fail_file_not_found)
			break;
		if (result == Level::load_fail_file_not_found || result == Level::load_fail_bad_format)
			return -1;
		m_level[x]->loadLevel(filenameSub, k);
	}

	for (unsigned m = 0; m < 6; m++)
	{
		vector<Wall*> temp;
		vector<ExtraLifeGoodie*> temp2;
		vector<RestoreHealthGoodie*> temp3;
		vector<AmmoGoodie*> temp4;
		vector<Hostage*> temp5;
		vector<Jewel*> temp6;
		vector<SnarlBotHorizontal*> temp7;
		vector<SnarlBotVertical*> temp8;
		vector<Factory*> temp9;
		vector<KleptoBot*> temp10;
		vector<Gate*> temp11;
		
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				Level::MazeEntry item = m_level[x]->getContentsOf(i, j, m);
				switch (item)
				{
				case Level::player:
					m_player[m] = new Player(this, i, j, m);
					break;
				case Level::wall:
					temp.push_back(new Wall(this, i, j, m));
					break;
				case Level::exit:
					m_exit = new Exit(this, i, j, m);
					break;
				case Level::extra_life:
					temp2.push_back(new ExtraLifeGoodie(this, i, j, m));
					break;
				case Level::restore_health:
					temp3.push_back(new RestoreHealthGoodie(this, i, j, m));
					break;
				case Level::ammo:
					temp4.push_back(new AmmoGoodie(this, i, j, m));
					break;
				case Level::hostage:
					temp5.push_back(new Hostage(this, i, j, m));
					numHostages++;
					break;
				case Level::jewel:
					temp6.push_back(new Jewel(this, i, j, m));
					numJewel++;
					break;
				case Level::gate0:
					temp11.push_back(new Gate(this, i, j, m,0));
					break;
				case Level::gate1:
					temp11.push_back(new Gate(this, i, j, m, 1));
					break;
				case Level::gate2:
					temp11.push_back(new Gate(this, i, j, m, 2));
					break;
				case Level::gate3:
					temp11.push_back(new Gate(this, i, j, m, 3));
					break;
				case Level::gate4:
					temp11.push_back(new Gate(this, i, j, m, 4));
					break;
				case Level::gate5:
					temp11.push_back(new Gate(this, i, j, m, 5));
					break;
				case Level::horiz_snarlbot:
					temp7.push_back(new SnarlBotHorizontal(this, i, j, m));
					break;
				case Level::vert_snarlbot:
					temp8.push_back(new SnarlBotVertical(this, i, j, m));
					break;
				case Level::kleptobot_factory:
				{
					temp9.push_back(new Factory(this, i, j, m));
					for (int l = 0; l < 3; l++)   // create kletpbots when first initialize the factory
					{
						temp10.push_back(new KleptoBot(this, i, j, m));
					}
				}
					break;
				default:
					break;
				}
			}
		}
		m_wall.push_back(temp);
		m_extralife.push_back(temp2);
		m_restorehealth.push_back(temp3);
		m_ammo.push_back(temp4);
		m_hostages.push_back(temp5);
		m_Jewel.push_back(temp6);
		m_snarlh.push_back(temp7);
		m_snarlv.push_back(temp8);
		m_factory.push_back(temp9);
		m_kleptobots.push_back(temp10);
		m_gate.push_back(temp11);
	}
	return  GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
	//decLives();
	//return GWSTATUS_PLAYER_DIED;

	//update game status line
	updateDisplayText();

	//Give each actor a chance to do something 
	// You need to make sure the current_level has the correct level info to indicate which level the actor resides in. 
	int sublevel = getCurrentSubLevel();

	//player object
	if (m_player[sublevel] != nullptr)
	{
		if (m_player[sublevel]->alive())
		{
			m_player[sublevel]->doSomething();
		}
	}

    //for wall object
	for (unsigned int i = 0; i < m_wall[sublevel].size(); i++)
	{
		if (m_wall[sublevel][i] != nullptr)
		{
			m_wall[sublevel][i]->doSomething();
		}


	}

	//for exit object


	//for Goodie object
	for (unsigned int i = 0; i < m_extralife[sublevel].size(); i++)
	{
		if (m_extralife[sublevel][i] != nullptr)
		{
			m_extralife[sublevel][i]->doSomething();
		}
	}

	for (unsigned int i = 0; i < m_restorehealth[sublevel].size(); i++)
	{
		if (m_restorehealth[sublevel][i] != nullptr)
		{
			m_restorehealth[sublevel][i]->doSomething();
		}

	}

	for (unsigned int i = 0; i < m_ammo[sublevel].size(); i++)
	{
		if (m_ammo[sublevel][i] != nullptr)
		{
			m_ammo[sublevel][i]->doSomething();
		}
	}

	for (unsigned int i = 0; i < m_Jewel[sublevel].size(); i++)
	{
		if (m_Jewel[sublevel][i] != nullptr)
		{
			m_Jewel[sublevel][i]->doSomething();
		}
	}

	for (unsigned int i = 0; i < m_hostages[sublevel].size(); i++)
	{
		if (m_hostages[sublevel][i] != nullptr)
		{
			m_hostages[sublevel][i]->doSomething();
		}
	}

	for (unsigned int i = 0; i < m_gate[sublevel].size(); i++)
	{
		if (m_gate[sublevel][i] != nullptr)
		{
			m_gate[sublevel][i]->doSomething();
		}
	}

	

	//for robot object
	for (unsigned int i = 0; i < m_factory[sublevel].size(); i++)
	{
		if (m_factory[sublevel][i] != nullptr)
		{
			m_factory[sublevel][i]->doSomething();
		}
	}

	unsigned int temp1 = m_kleptobots[sublevel].size();
	for (unsigned int i = 0; i < temp1; i++)
	{
		if (m_kleptobots[sublevel][i] != nullptr)
		{
			m_kleptobots[sublevel][i]->doSomething();
		}
	}

	for (unsigned int i = 0; i < m_snarlh[sublevel].size(); i++)
	{
		if (m_snarlh[sublevel][i] != nullptr)
		{
			m_snarlh[sublevel][i]->doSomething();
		}
	}

	for (unsigned int i = 0; i < m_snarlv[sublevel].size(); i++)
	{
		if (m_snarlh[sublevel][i] != nullptr)
		{
			m_snarlv[sublevel][i]->doSomething();
		}
	}

	unsigned temp2 = m_bullet.size();
	for (unsigned int i = 0; i < temp2; i++)
	{
		if (m_bullet[i] != nullptr)
		{
			m_bullet[i]->doSomething();
		}
	}

	//remove dead
	for (unsigned int i = 0; i < m_extralife[sublevel].size(); i++)
	{
		if (m_extralife[sublevel][i]->alive() == false)
		{
			delete m_extralife[sublevel][i];
			m_extralife[sublevel].erase(m_extralife[sublevel].begin() + i);
		}
	}

	for (unsigned int i = 0; i < m_ammo[sublevel].size(); i++)
	{
		if (m_ammo[sublevel][i]->alive() == false)
		{
			delete m_ammo[sublevel][i];
			m_ammo[sublevel].erase(m_ammo[sublevel].begin() + i);
		}
	}

	for (unsigned int i = 0; i < m_restorehealth[sublevel].size(); i++)
	{
		if (m_restorehealth[sublevel][i]->alive() == false)
		{
			delete m_restorehealth[sublevel][i];
			m_restorehealth[sublevel].erase(m_restorehealth[sublevel].begin() + i);
		}
 	}

	for (unsigned int i = 0; i < m_Jewel[sublevel].size(); i++)
	{
		if (m_Jewel[sublevel][i]->alive() == false)
		{
			delete m_Jewel[sublevel][i];
			numJewel--;
			m_Jewel[sublevel].erase(m_Jewel[sublevel].begin() + i);
		}
	}

	for (unsigned int i = 0; i < m_hostages[sublevel].size(); i++)
	{
		if (m_hostages[sublevel][i]->alive() == false)
		{
			delete m_hostages[sublevel][i];
			m_hostages[sublevel].erase(m_hostages[sublevel].begin() + i);
			numHostages--;
		}
	}


	for (unsigned int i = 0; i < m_gate[sublevel].size(); i++)
	{
		if (m_gate[sublevel][i]->alive() == false)
		{
			delete m_gate[sublevel][i];
			m_gate[sublevel].erase(m_gate[sublevel].begin() + i);
			
			
		}
	}

	for (unsigned int i = 0; i < m_kleptobots[sublevel].size(); i++)
	{
		if (m_kleptobots[sublevel][i]->alive() == false)
		{
			delete m_kleptobots[sublevel][i];
			m_kleptobots[sublevel].erase(m_kleptobots[sublevel].begin() + i);

		}
	}

	for (unsigned int i = 0; i < m_snarlh[sublevel].size(); i++)
	{
		if (m_snarlh[sublevel][i]->alive() == false)
		{
			delete m_snarlh[sublevel][i];
			m_snarlh[sublevel].erase(m_snarlh[sublevel].begin() + i);
		}
	}

	for (unsigned int i = 0; i < m_snarlv[sublevel].size(); i++)
	{
		if (m_snarlv[sublevel][i]->alive() == false)
		{
			delete m_snarlv[sublevel][i];
			m_snarlv[sublevel].erase(m_snarlv[sublevel].begin() + i);
		}
	}

	for (unsigned int i = 0; i < m_bullet.size(); i++)
	{
		if (m_bullet[i]->alive() == false)
		{
			delete m_bullet[i];
			m_bullet.erase(m_bullet.begin()+i);
		}
	}


	//check status whether advance level
	if (advanceLevel())
		m_exit->Revealed();

	if (getActorAtLocation(getPlayer()->getX(),getPlayer()->getY())->getID() == IID_EXIT && m_exit->returnvisibility() == true && getCurrentSubLevel() == 0)
	{
		m_exit->doSomething();
		increaseScore(Tick);
		return  GWSTATUS_FINISHED_LEVEL;
	}
	// Reduce the Time Limit by one
	Tick--;

	//return the proper result
	if (!m_player[sublevel]->alive())
	{
		if (getLives() > 0)
		{
			changeSublevel(0);
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
		else
			return GWSTATUS_PLAYER_DIED;
	}

	if (Tick == 0)
	{
		if (getLives() > 0)
		{
			changeSublevel(0);
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
		else
			return GWSTATUS_PLAYER_DIED;
	}



	return GWSTATUS_CONTINUE_GAME;



}

void StudentWorld::updateDisplayText()
{
	int score = getScore(); 
	int level = getLevel();
	int sublevel = getCurrentSubLevel();
	unsigned int timeLimits = getCurrentTimeLimit();
	int livesLeft = getLives();
	int health = m_player[getCurrentSubLevel()]->getHealth();
	int ammo = m_player[getCurrentSubLevel()]->getAmmo();
	string s = setString(score, level, sublevel, livesLeft, health,ammo,timeLimits);
	setGameStatText(s);
}

int StudentWorld::getCurrentTimeLimit()
{
	return Tick;
}


//write the display string
string StudentWorld::setString(int score, int level, int sublevel, int lives, int health, int ammo, int bonus)
{
	ostringstream oss;
	oss.fill('0');
	oss << "Score: "<< setw(7) << score;
	oss << "  Level: "<< setw(2) << level << '-'<<setw(2) << sublevel;
	oss.fill(' ');
	oss << "  Lives: " << setw(2) << lives;
	oss << "  Health: " << setw(3) << health << '%';
	oss << "  Ammo: " << setw(3) << ammo;
	oss << "  TimeLimit: " << setw(4) << bonus;
	string m = oss.str();
	return m;
}

//clean up
void StudentWorld::cleanUp()
 {
	 delete m_exit;

	 for (int i = 0; i < 5; i++)
	 {
		 delete m_player[i];
		 m_player[i] = nullptr;
	 }


	for (unsigned int i = 0; i < m_bullet.size(); i++)
	{
		{
			delete m_bullet[i];
			m_bullet[i] = nullptr;
		}
	}
	m_bullet.clear();
	

	for (unsigned int i = 0; i < 6; i++)
	{
		for (unsigned int j = 0; j < m_wall[i].size(); j++)
		{
			
			    delete m_wall[i][j];
				m_wall[i][j] = nullptr;
			
		}
	}
	m_wall.clear();

	for (unsigned int i = 0; i < m_extralife.size(); i++)
	{
		for (unsigned int j = 0; j < m_extralife[i].size(); j++)
		{
			delete m_extralife[i][j];
			m_extralife[i][j] = nullptr;
		}
	}
	m_extralife.clear();

	for (unsigned int i = 0; i < m_restorehealth.size(); i++)
	{
		for (unsigned int j = 0; j < m_restorehealth[i].size(); j++)
		{
			delete m_restorehealth[i][j];
			m_restorehealth[i][j] = nullptr;
		}
	}
	m_restorehealth.clear();
   

	for (unsigned int i = 0; i < m_ammo.size(); i++)
	{
		for (unsigned int j = 0; j < m_ammo[i].size(); j++)
		{
			delete m_ammo[i][j];
			m_ammo[i][j] = nullptr;
		}
	}
	m_ammo.clear();


	for (unsigned int i = 0; i < m_hostages.size(); i++)
	{
		for (unsigned int j = 0; j < m_hostages[i].size(); j++)
		{
			delete m_hostages[i][j];
			m_hostages[i][j] = nullptr;
		}
	}
	m_hostages.clear();

	for (unsigned int i = 0; i < m_Jewel.size(); i++)
	{
		for (unsigned int j = 0; j < m_Jewel[i].size(); j++)
		{
			delete m_Jewel[i][j];
			m_Jewel[i][j] = nullptr;
		}
	}
	m_Jewel.clear();

	for (unsigned int i = 0; i < m_gate.size(); i++)
	{
		for (unsigned int j = 0; j < m_gate[i].size(); j++)
		{
			delete m_gate[i][j];
			m_gate[i][j] = nullptr;
		}
	}
	m_gate.clear();

	for (unsigned int i = 0; i < m_snarlh.size(); i++)
	{
		for (unsigned int j = 0; j < m_snarlh[i].size(); j++)
		{
			delete m_snarlh[i][j];
			m_snarlh[i][j] = nullptr;
		}
	}
	m_snarlh.clear();

	for (unsigned int i = 0; i < m_snarlv.size(); i++)
	{
		for (unsigned int j = 0; j < m_snarlv[i].size(); j++)
		{
			delete m_snarlv[i][j];
			m_snarlv[i][j] = nullptr;
		}
	}
	m_snarlv.clear();

	for (unsigned int i = 0; i < m_kleptobots.size(); i++)
	{
		for (unsigned int j = 0; j < m_kleptobots[i].size(); j++)
		{
			delete m_kleptobots[i][j];
			m_kleptobots[i][j] = nullptr;
		}
	}
	m_kleptobots.clear();

	for (unsigned int i = 0; i < m_factory.size(); i++)
	{
		for (unsigned int j = 0; j < m_factory[i].size(); j++)
		{
			delete  m_factory[i][j];
			m_factory[i][j] = nullptr;
		}
	}
	m_factory.clear();

}

//destructor
StudentWorld::~StudentWorld()
{
	cleanUp();
}


//get current sublevel
int StudentWorld::getCurrentSubLevel()
{
	return currentSubLevel;
}

//change sublevel
void StudentWorld::changeSublevel(int i)
{
	int m = getCurrentSubLevel();
	 int ammo = m_player[m]->getAmmo();
	 int hitpoints = m_player[m]->getHitPoints();
	currentSubLevel = i;
	m_player[i]->assignAmmu(ammo);
	m_player[i]->assignHit(hitpoints);
	
}

//get player pointer
Player* StudentWorld::getPlayer()
{
	return m_player[getCurrentSubLevel()];
}

//get an actor at location
Actor* StudentWorld::getActorAtLocation(int x, int y)
{
	int m = getCurrentSubLevel();
	//wall
	for (unsigned int i = 0; i < m_wall[m].size(); i++)
	{
		if (x == m_wall[m][i]->getX() && y == m_wall[m][i]->getY() )
			return m_wall[m][i]->returnMyself();
	}

	//exit
		if (x == m_exit->getX() && y == m_exit->getY() )
			return m_exit->returnMyself();

	//extralife goodie
	for (unsigned int i = 0; i < m_restorehealth[m].size(); i++)
	{
		if (x == m_restorehealth[m][i]->getX() && y == m_restorehealth[m][i]->getY() )
			return m_restorehealth[m][i]->returnMyself();
	}

	//restorehealth goodie
	for (unsigned int i = 0; i < m_restorehealth[m].size(); i++)
	{
		if (x == m_restorehealth[m][i]->getX() && y == m_restorehealth[m][i]->getY() )
			return m_restorehealth[m][i]->returnMyself();
	}

	//ammo goodie
	for (unsigned int i = 0; i < m_ammo[m].size(); i++)
	{
		if (x == m_ammo[m][i]->getX() && y == m_ammo[m][i]->getY() )
			return m_ammo[m][i]->returnMyself();
	}

	//hostages
	for (unsigned int i = 0; i < m_hostages[m].size(); i++)
	{
		if (x == m_hostages[m][i]->getX() && y == m_hostages[m][i]->getY())
			return  m_hostages[m][i]->returnMyself();
	}

	//jewels
	for (unsigned int i = 0; i < m_Jewel[m].size(); i++)
	{
		if (x == m_Jewel[m][i]->getX() && y == m_Jewel[m][i]->getY() )
			return  m_Jewel[m][i]->returnMyself();
	}



	//player
	if (x == m_player[getCurrentSubLevel()]->getX() && y == m_player[getCurrentSubLevel()]->getY())
		return m_player[getCurrentSubLevel()]->returnMyself(); 


	//gate
	for (unsigned int i = 0; i < m_gate[m].size(); i++)
	{
		if (x == m_gate[m][i]->getX() && y == m_gate[m][i]->getY())
			return m_gate[m][i]->returnMyself();
	}
	

	


	//snarl horizontal robots
	for (unsigned int i = 0; i < m_snarlh[m].size(); i++)
	{
		if (x == m_snarlh[m][i]->getX() && y == m_snarlh[m][i]->getY())
			return  m_snarlh[m][i]->returnMyself();
	}

	//snarl vertical robots
	for (unsigned int i = 0; i < m_snarlv[m].size(); i++)
	{
		if (x == m_snarlv[m][i]->getX() && y == m_snarlv[m][i]->getY())
			return  m_snarlv[m][i]->returnMyself();
	}

	//kleptoBot
	for (unsigned int i = 0; i < m_kleptobots[m].size(); i++)
	{
		if (x == m_kleptobots[m][i]->getX() && y == m_kleptobots[m][i]->getY())
			return  m_kleptobots[m][i]->returnMyself();
	}

	//factory
	for (unsigned int i = 0; i < m_factory[m].size(); i++)
	{
		if (x == m_factory[m][i]->getX() && y == m_factory[m][i]->getY())
			return  m_factory[m][i]->returnMyself();
	}

	//bullet
	for (unsigned int i = 0; i < m_bullet.size(); i++)
	{
		if (x == m_bullet[i]->getX() && y == m_bullet[i]->getY())
			return m_bullet[i]->returnMyself();
	}

	return nullptr;
}

//get a robot pointer
Robot* StudentWorld::getRobot(int x, int y)
{

	int m = getCurrentSubLevel();

	//snarl horizontal robots
	for (unsigned int i = 0; i < m_snarlh[m].size(); i++)
	{
		if (x == m_snarlh[m][i]->getX() && y == m_snarlh[m][i]->getY())
			return  m_snarlh[m][i]->returnRobot();
	}
	//snarl vertical robots
	for (unsigned int i = 0; i < m_snarlv[m].size(); i++)
	{
		if (x == m_snarlv[m][i]->getX() && y == m_snarlv[m][i]->getY())
			return  m_snarlv[m][i]->returnRobot();
	}

	//kleptoBot
	for (unsigned int i = 0; i < m_kleptobots[m].size(); i++)
	{
		if (x == m_kleptobots[m][i]->getX() && y == m_kleptobots[m][i]->getY())
			return  m_kleptobots[m][i]->returnRobot();
	}

	return nullptr;

}

//manage the pick up Goodie issue
void StudentWorld::pickGoodie(Actor* ap)
{
	if (ap->getID() == IID_AMMO)
	{
		int m = getCurrentSubLevel();
		for (unsigned int i = 0; i < m_ammo[m].size(); i++)
		{
			if (m_ammo[m][i]->getX() == ap->getX() && m_ammo[m][i]->getY() == ap->getY())
			{
				AmmoGoodie* temp = m_ammo[m][i];
				m_ammo[m].erase(m_ammo[m].begin() + i);
				delete temp;
			}    
		}
	}

	if (ap->getID() == IID_RESTORE_HEALTH)
	{
		int m = getCurrentSubLevel();
		for (unsigned int i = 0; i < m_restorehealth[m].size(); i++)
		{
			if (m_restorehealth[m][i]->getX() == ap->getX() && m_restorehealth[m][i]->getY() == ap->getY())
			{
				RestoreHealthGoodie* temp = m_restorehealth[m][i];
				m_restorehealth[m].erase(m_restorehealth[m].begin() + i);
				delete temp;
			}
		}
	}

	if (ap->getID() == IID_EXTRA_LIFE)
	{
		int m = getCurrentSubLevel();
		for (unsigned int i = 0; i < m_extralife[m].size(); i++)
		{
			if (m_extralife[m][i]->getX() == ap->getX() && m_extralife[m][i]->getY() == ap->getY())
			{
				ExtraLifeGoodie* temp =  m_extralife[m][i];
				m_extralife[m].erase(m_extralife[m].begin() + i);
				delete temp;
			}
		}
	}
	return;
}

//add a Goodie after the kleptoBot died
void StudentWorld::addGoodie(Actor* ap)
{
	int m = getCurrentSubLevel();
	int x = ap->getX();
	int y = ap->getY();

	if (ap->getID() == IID_AMMO)
	{
		m_ammo[m].push_back(new AmmoGoodie(this, x, y, m));
		return;
	}

	if (ap->getID() == IID_EXTRA_LIFE)
	{
		m_extralife[m].push_back(new ExtraLifeGoodie(this, x, y, m));
		return;
	}

	if (ap->getID() == IID_RESTORE_HEALTH)
	{
		m_restorehealth[m].push_back(new RestoreHealthGoodie(this, x, y, m));
		return;
	}

	return;
}

//add kelptoBots
void StudentWorld::addKleptoBots(Factory* ap)
{
	int m = getCurrentSubLevel();
	m_kleptobots[m].push_back(new KleptoBot(this, ap->getX(), ap->getY(), m));
}

//add Bullet
void StudentWorld::addBullets(int x, int y, Actor* ap)
{
	int m = getCurrentSubLevel();
	m_bullet.push_back(new Bullet(this, x, y, ap->getDirection(), m));
}

//add a player
void StudentWorld::addPlayer(int Sub)
{
	return;
}

//count kleptoBot
int StudentWorld::countKlpto(int x, int y)
{
	int count = 0;
	for (unsigned int i = 0; i < m_kleptobots[getCurrentSubLevel()].size(); i++)
	{
		if (x == m_kleptobots[getCurrentSubLevel()][i]->getX() && y == m_kleptobots[getCurrentSubLevel()][i]->getY())
		count++;
	}

	return count;
}

//detect whether advance a level
bool StudentWorld::advanceLevel()
{
	if (numHostages == 0 && numJewel == 0)
		return true;
	else
		return false;
}
