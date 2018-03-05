// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

#include "Actor.h"
#include "StudentWorld.h"
#include "Level.h"
#include <cmath>


//Actor class implementation
StudentWorld* Actor::getWorld()
{
	return m_ptr;
}

bool Actor::alive()
{
	return m_life;
}

void Actor::setDead()
{
	m_life = false;
}

Actor* Actor::returnMyself()
{
	if (this != nullptr)
	{
		return this;
	}
}

bool Actor::ckeckObstacle(int x, int y)
{
	Actor* ap = getWorld()->getActorAtLocation(x, y);
	if (ap != nullptr)
	{
		if (ap->alive())
		{
			if (ap->getID() == IID_WALL)
				return false;
			else if (ap->getID() == IID_KLEPTOBOT)
				return false;
			else if (ap->getID() == IID_ROBOT_FACTORY)
				return false;
			else if (ap->getID() == IID_SNARLBOT)
				return false;
			else if (ap->getID() == IID_PLAYER)
				return false;
			else
				return true;
		}
		else
			return false;
	}
	else
		return true;
}

void Actor::doSomething()
{
	if (!alive())
		return;
}

void Actor::Attacked()
{
	return;
}

Actor::~Actor(){ return; };

//Player class implementation

void Player::resetHitPoints()
{
 m_hitPoints = 20;
}

void Player::decreaseHitPoints(int howmuch)
{
	m_hitPoints -= howmuch;
}

void Player::increaseAmmunition(int number)
{
	m_ammunition += number;
}

int Player::getHealth()
{
	return (m_hitPoints*5);
}

int Player::getHitPoints()
{
	return m_hitPoints;
}

int Player::getAmmo()
{
	return m_ammunition;
}

void Player::assignHit(int newH)
{
	m_hitPoints = newH;
}

void Player::assignAmmu(int newA)
{
	m_ammunition = newA;
}

void Player::doSomething()
{
	Actor::doSomething();

	int ch;
	
	//user hit a key this case
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_ESCAPE:
			setDead();
			break;
		case KEY_PRESS_SPACE:
		{
			if (m_ammunition > 0)
	     {
			m_ammunition--;
			int x = getX();
			int y = getY();
			Direction playerd = getDirection();
			switch (playerd)
			{
			case up:
				y++;
				break;
			case down:
				y--;
				break;
			case right:
				x++;
				break;
			case left:
				x--;
				break;
			default:
				break;
			}
			getWorld()->addBullets(x, y, this);
			getWorld()->playSound(4);
		}
		break;
		}

		case  KEY_PRESS_LEFT:
		{	
		 	setDirection(left);
		    if (ckeckObstacle(getX() - 1, getY()))
			  moveTo(getX() - 1, getY());
		    break;
		}
		
		case KEY_PRESS_RIGHT:
		{
			setDirection(right);
		    if (ckeckObstacle(getX() + 1, getY()))
		        moveTo(getX() + 1, getY());
		    break;
		}
		case KEY_PRESS_DOWN:
		{
			setDirection(down);
		    if (ckeckObstacle(getX(), getY()-1))
			   moveTo(getX(), getY()-1);
		    break;
		}
		case KEY_PRESS_UP:
		{
			setDirection(up);
		    if (ckeckObstacle(getX(), getY()+1))
		       moveTo(getX(), getY()+1);
		    break;
	    }
		default:
			break;
		}
	}
}

void Player::Attacked()
{
	decreaseHitPoints(2);

	if (m_hitPoints > 0)
		getWorld()->playSound(10);
	else
	{
		setDead();
		getWorld()->playSound(2);
	}
}

//Wall class implementation
void Wall::doSomething()
{
	return;
}

void Wall::Attacked()
{
	return;
}

//Bullet class implementation
void Bullet::doSomething()
{
	if (!alive())
		return;

	if (getWorld()->getActorAtLocation(getX(), getY()) != nullptr)
	{
		if (getWorld()->getActorAtLocation(getX(), getY())->getID() == IID_PLAYER)
		{
			getWorld()->getPlayer()->Attacked(); 
			setDead();
			return;
		}
		else if (getWorld()->getActorAtLocation(getX(), getY())->getID() == IID_KLEPTOBOT || getWorld()->getActorAtLocation(getX(), getY())->getID() == IID_SNARLBOT)
		{
			getWorld()->getRobot(getX(), getY())->damaged();
			setDead();
			return;
		}
		else if (getWorld()->getActorAtLocation(getX(), getY())->getID() == IID_WALL)
		{
			setDead();
			return;
		}
		else if (getWorld()->getActorAtLocation(getX(), getY())->getID() == IID_ROBOT_FACTORY)
		{
			if (getWorld()->countKlpto(getX(), getY()) > 0)
			{
				getWorld()->getRobot(getX(), getY())->damaged();
				setDead();
				return;
			}
			else
			{
				setDead();
				return;
			}
		}
	}

	Direction dir = getDirection();
	switch (dir)
	{
	case up:
		moveTo(getX(), getY() + 1);
		break;
	case right:
		moveTo(getX() + 1, getY());
		break;
	case down:
		moveTo(getX(), getY() - 1);
		break;
	case left:
		moveTo(getX() - 1, getY());
		break;
	default:
		break;
	}
	if (getWorld()->getActorAtLocation(getX(), getY()) != nullptr)
	{
		if (getWorld()->getActorAtLocation(getX(), getY())->getID() == IID_PLAYER)
		{
			getWorld()->getPlayer()->Attacked(); 
			setDead();
			return;
		}
		else if (getWorld()->getActorAtLocation(getX(), getY())->getID() == IID_KLEPTOBOT || getWorld()->getActorAtLocation(getX(), getY())->getID() == IID_SNARLBOT)
		{
			getWorld()->getRobot(getX(), getY())->damaged();
			setDead();
			return;
		}
		else if (getWorld()->getActorAtLocation(getX(), getY())->getID() == IID_WALL)
		{
			setDead();
			return;
		}
		else if (getWorld()->getActorAtLocation(getX(), getY())->getID() == IID_ROBOT_FACTORY)
		{
			if (getWorld()->countKlpto(getX(), getY()) > 0)
			{
				getWorld()->getRobot(getX(), getY())->damaged();
				setDead();
				return;
			}
			else
			{
				setDead();
				return;
			}
		}
	}
}

void Bullet::Attacked()
{
	return;
}

//Exit class
void Exit::Revealed()
{
	if (visibility == false)
	{
		visibility = true;
		setVisible(true);
		getWorld()->playSound(SOUND_REVEAL_EXIT);
	}
	else
		return;
}

bool Exit::returnvisibility()
{
	if (visibility == true)
		return true;
	else
		return false;
}

void Exit::doSomething()
{
	if (getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY() && visibility == true)
	{
		getWorld()->playSound(SOUND_FINISHED_LEVEL);
		getWorld()->increaseScore(1000);

	}
}

void Exit::Attacked()
{
	return;
}

//Goodie class and its derived class implementation

void Goodie::doSomething()
{
	//check wheather still alive
	Actor::doSomething();

	//check whether player step on the item
	if (getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY())
	{
		setDead();
		getWorld()->playSound(5);
	}

}

void Goodie::Attacked()
{
	return;
}

//ExtraLifeGoodie
void ExtraLifeGoodie::doSomething()
{
	Goodie::doSomething();
	if (!alive())
	{
		getWorld()->increaseScore(500);
		getWorld()->incLives();
	}
}

//RestoreHealthGoogie
void RestoreHealthGoodie::doSomething()
{
	Goodie::doSomething();
	if (!alive())
	{
		getWorld()->increaseScore(1000);
		getWorld()->getPlayer()->resetHitPoints();
	}
}

//AmmoGoodie
void AmmoGoodie::doSomething()
{
	Goodie::doSomething();
	if (!alive())
	{
		getWorld()->increaseScore(200);
		getWorld()->getPlayer()->increaseAmmunition(25);
	}
}

//Hostage class
void Hostage::doSomething()
{
	Goodie::doSomething();
}

//Jewel class
void Jewel::doSomething()
{
	Goodie::doSomething();
	if (!alive())
	{
		getWorld()->increaseScore(100);
	}
}

//Gate class
void Gate::doSomething()
{
	Goodie::doSomething();
	if (!alive())
	{
		getWorld()->changeSublevel(sublevelTransportTo);
	}
}

//Robot class and its derived class implementation
int Robot::getHealth()
{
	return m_hitPoints;
}

int Robot::computrFrequency()
{
	int x =getWorld()->getLevel();
	int ticks = (28 - x) / 4;
	if (ticks < 3)
		ticks = 3;
	return ticks;
}

Robot* Robot::returnRobot()
{
	return this;
}

bool Robot::returnRest()
{
	return rest;
}

void Robot::doSomething()
{
	if (!alive())
		return;

	if (tickCount != computrFrequency())
	{
		tickCount++;
		rest = true;
		return;
	}
	else
	{
		tickCount = 0;
		rest = false;
	}
}

void Robot::damaged()
{
	m_hitPoints -= 2; 

	if (m_hitPoints >0)
		getWorld()->playSound(9);
	else
	{
		setDead();
		getWorld()->playSound(1);
	}
}

//snarlrobot
void SnarlBotHorizontal::doSomething()
{
	Robot::doSomething();

	if (returnRest() == false)
	{
		bool fire = true;
		bool direc = false;
		if (getY() == getWorld()->getPlayer()->getY())  // same row as player
		{
			int x = getWorld()->getPlayer()->getX();
			if (x < getX() && getDirection() == left)
				direc = true;
			if (x > getX() && getDirection() == right)
				direc = true;

			if (direc == true)
			{
				for (int i = 0; i < 15; i++)
				{
					if (!ckeckObstacle(i, getY()))
					{
						if ((i <= getX() && i <= x) || (i >= x && i >= getX()))
						{
							fire = true;
							continue;
						}
						else
						{
							fire = false;
							break;
						}
					}
				}

				if (fire == true)    
				{
					Direction dir = getDirection();
					switch (dir)
					{
					case left:
						getWorld()->addBullets(getX() - 1, getY(), this);
					case right:
						getWorld()->addBullets(getX() + 1, getY(), this);
					}

					getWorld()->playSound(3);
					return;
				}
			}
		}

		Direction dir = getDirection();
		switch (dir)
		{
		case left:
		{
			if (ckeckObstacle(getX() - 1, getY()))
				moveTo(getX() - 1, getY());
			else
				setDirection(right);
			break;
		}
		case right:
		{if (ckeckObstacle(getX() + 1, getY()))
			moveTo(getX() + 1, getY());
		else
			setDirection(left);
		break;
		}
		default:
			break;
		}
	}
	
	return;
}

void SnarlBotHorizontal::damaged()
{
	Robot::damaged();
	if (!alive())
	{
		getWorld()->increaseScore(200);
	}
}

void SnarlBotVertical::doSomething()
{
	Robot::doSomething();

	if (returnRest() == false)
	{
		bool fire = true;
		bool direc = false;
		if (getX() == getWorld()->getPlayer()->getX())  // same column as player
		{
			int y = getWorld()->getPlayer()->getY();
			if (y < getY() && getDirection() == down)
				direc = true;
			if (y > getY() && getDirection() == up)
				direc = true;

			if (direc == true)
			{
				for (int i = 0; i < 15; i++)
				{
					if (!ckeckObstacle(i, getY()))
					{
						if ((i <= getY() && i <= y) || (i >= y && i >= getY()))
						{
							fire = true;
							continue;
						}
						else
						{
							fire = false;
							break;
						}
					}
					else
						continue;
				}

				if (fire == true)    // implement later, the bullet object
				{
					Direction dir = getDirection();
					switch (dir)
					{
					case up:
						getWorld()->addBullets(getX(), getY() + 1, this);
					case down:
						getWorld()->addBullets(getX(), getY() - 1, this);
					}
					getWorld()->playSound(3);
					return;
				}
			}
		}

		Direction dir = getDirection();
		switch (dir)
		{
		case down:
		{
			if (ckeckObstacle(getX(), getY() - 1))
				moveTo(getX(), getY() - 1);
			else
				setDirection(up);
			break;
		}
		case up:
		{if (ckeckObstacle(getX(), getY() + 1))
			moveTo(getX(), getY() + 1);
		else
			setDirection(down);
		break;
		}
		default:
			break;
		}
	}

	return;
}

void SnarlBotVertical::damaged()
{
	Robot::damaged();
	if (!alive())
	{
		getWorld()->increaseScore(200);
	}
}

//kelptoBot
bool KleptoBot::pickGoodieChance()
{
	int i = rand() % 5;
	switch (i)
	{
	case 0:
		return false;
		break;
	case 1:
		return true;
		break;
	case 2:
		return false;
		break;
	case 3:
		return false;
		break;
	case 4:
		return false;
		break;
	default:
		return false;
		break;
	}
}

void KleptoBot::doSomething()
{
	Robot::doSomething();

	if (returnRest() == false)
	{
		if (alive())

		{
			Actor* ap = getWorld()->getActorAtLocation(getX(), getY());

			if (pickGoodieChance() && pickGoodieAlready == false)
			{
				if (ap != nullptr && ap->getID() == IID_AMMO)
				{
					m_treasure = new AmmoGoodie(getWorld(), getX(), getY(), getWorld()->getCurrentSubLevel());
					m_treasure->setVisible(false);
					pickGoodieAlready = true;
					getWorld()->pickGoodie(ap);
					getWorld()->playSound(SOUND_ROBOT_MUNCH);
					return;
				}
				else if (ap != nullptr && ap->getID() == IID_RESTORE_HEALTH)
				{
					m_treasure = new RestoreHealthGoodie(getWorld(), getX(), getY(), getWorld()->getCurrentSubLevel());
					m_treasure->setVisible(false);
					pickGoodieAlready = true;
					getWorld()->pickGoodie(ap);
					getWorld()->playSound(SOUND_ROBOT_MUNCH);
					return;
				}
				else if (ap != nullptr && ap->getID() == IID_EXTRA_LIFE)
				{
					m_treasure = new ExtraLifeGoodie(getWorld(), getX(), getY(), getWorld()->getCurrentSubLevel());
					m_treasure->setVisible(false);
					pickGoodieAlready = true;
					getWorld()->pickGoodie(ap);
					getWorld()->playSound(SOUND_ROBOT_MUNCH);
					return;
				}

			}

			if (count < distanceBeforeTurning)
			{
				Direction direc = getDirection();
				switch (direc)
				{
				case left:
					if (ckeckObstacle(getX() - 1, getY()))
					{
						moveTo(getX() - 1, getY());
						count++;
						return;
					}
					break;
				case right:
					if (ckeckObstacle(getX() + 1, getY()))
					{
						moveTo(getX() + 1, getY());
						count++;
						return;
					}
					break;
				case up:
					if (ckeckObstacle(getX(), getY() + 1))
					{
						moveTo(getX(), getY() + 1);
						count++;
						return;
					}
					break;
				case down:
					if (ckeckObstacle(getX(), getY() - 1))
					{
						moveTo(getX(), getY() - 1);
						count++;
						return;
					}
					break;
				default:
					break;
				}
			}

			count = 0;

			int k = rand() % 3;
			switch (k)
			{
			case 0: distanceBeforeTurning = 1; break;
			case 1: distanceBeforeTurning = 2; break;
			case 2: distanceBeforeTurning = 3; break;
			default:break;
			}

			int d = rand() % 4;
			Direction temp;
			switch (d)
			{
			case 0: temp = left; break;
			case 1: temp = right; break;
			case 2: temp = up; break;
			case 3: temp = down; break;
			default: break;
			}

			for (int i = 0; i < 4; i++)
			{
				if (temp == left)
				{
					if (ckeckObstacle(getX() - 1, getY()))
					{
						setDirection(left);
						moveTo(getX() - 1, getY());
						return;
					}
					else
						temp = up;
				}
				if (temp == up)
				{
					if (ckeckObstacle(getX(), getY() + 1))
					{
						setDirection(up);
						moveTo(getX(), getY() + 1);
						return;
					}
					else temp = right;
				}
				if (temp == right)
				{
					if (ckeckObstacle(getX() + 1, getY()))
					{
						setDirection(right);
						moveTo(getX() + 1, getY());
						return;
					}
					else
						temp = down;
				}
				if (temp == down)
				{
					if (ckeckObstacle(getX(), getY() - 1))
					{
						setDirection(down);
						moveTo(getX(), getY() - 1);
						return;
					}
					else
						temp = left;
				}
			}

			setDirection(temp);
		}
	}

	return;
}

void KleptoBot::damaged()
{
	Robot::damaged();

	if (!alive())
	{
		if (pickGoodieAlready)
		{
			getWorld()->addGoodie(m_treasure->returnMyself());
			delete m_treasure;
			m_treasure = nullptr;
		}

		getWorld()->increaseScore(20);
	}
}

//factory
void Factory::doSomething()
{
	int x = getX();
	int xup = x+3;
	if (xup > 14)
		xup = 14;
	int xl = x - 3;
	if (xl < 0)
		xl = 0;
	int y = getY();
	int yup = y + 3;
	if (yup > 14)
		yup = 14;
	int yl = y - 3;
	if (yl < 0)
		yl = 0;
	int count = 0;

	for (int i = xl; i <= xup; i++)
	{
		for (int j = yl; j <= yup; j++)
		{
			count += getWorld()->countKlpto(i, j);
		}
	}

	if (getWorld()->getActorAtLocation(x, y) != nullptr)
	{
		if (count < 3 && getWorld()->getActorAtLocation(x, y)->getID() != IID_KLEPTOBOT)
			if ((rand() % 50) == 0)
			{
				getWorld()->addKleptoBots(this);
				getWorld()->playSound(SOUND_ROBOT_BORN);
			}
	}
}

void Factory::Attacked()
{
	return;
}




