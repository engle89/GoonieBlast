#ifndef GAMEWORLD_H_
#define GAMEWORLD_H_

#include "GameConstants.h"
#include <string>

const int START_PLAYER_LIVES = 3;

class GameController;

class GameWorld
{
public:

	GameWorld(std::string assetDir)
	 : m_lives(START_PLAYER_LIVES), m_score(0), m_level(0),
	   m_controller(nullptr), m_assetDir(assetDir)
	{
	}

	virtual ~GameWorld()
	{
	}
	
	virtual int init() = 0;
	virtual int move() = 0;
	virtual void cleanUp() = 0;
	virtual int getCurrentSubLevel() = 0;

	// specify what text is displayed at the top of the game screen
	void setGameStatText(std::string text);

	// determine if the user has hit a key on the keyboard to move the
	//Player, to fire, or to sacrifice one life and restart the main level.
	bool getKey(int& value);
	void playSound(int soundID);

	//determine the current main level number
	unsigned int getLevel() const
	{
		return m_level;
	}

	//determine how many lives the Player has lef
	unsigned int getLives() const
	{
		return m_lives;
	}
	
	// reduces the number of Player lives by one
	void decLives()
	{
		m_lives--;
	}
	
	// increases the number of Player lives by one
	void incLives() {
		m_lives++;
	}

	// determine the Player¡¯s current score
	unsigned int getScore() const
	{
		return m_score;
	}
	
	//used by a StudentWorld object (or your other classes) to increase the  
	//user¡¯s score upon successfully destroying a robot, picking up a Goodie of some sort, or 
	//completing a level (to give the Player their remaining level bonus). 
	// you must specify how many points the user gets
	void increaseScore(unsigned int howMuch)
	{
		m_score += howMuch;
	}
	
	  // The following should be used by only the framework, not the student

	bool isGameOver() const
	{
		return m_lives == 0;
	}

	void advanceToNextLevel() 
	{
		++m_level;
	}
   
	void setController(GameController* controller)
	{
		m_controller = controller;
	}

	// returns the name of the directory that contains the game assets (image,
	std::string assetDirectory() const
	{
		return m_assetDir;
	}
	
private:
	unsigned int	m_lives;
	unsigned int	m_score;
	unsigned int	m_level;
	GameController* m_controller;
	std::string		m_assetDir;
};

#endif // GAMEWORLD_H_
