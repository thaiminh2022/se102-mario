#include "StatManager.h"

StatManager* StatManager::_instance = nullptr;

StatManager* StatManager::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new StatManager();
	}
	return _instance;
}
StatManager::StatManager()
{
	Reset(); // Set initial values when created
}

void StatManager::Reset()
{
	coinCount = 0;
	score = 0;
	lifeCount = 3; // Starting lives
}


void StatManager::AddScore(int addingScore)
{
	score += addingScore;
}

void StatManager::AddCoin(int addingCoin)
{
	coinCount+= addingCoin;
}

void StatManager::AddLife(int addingLife)
{
	lifeCount += addingLife;
}

void StatManager::SetLevel(int newLevel)
{
	currentLevel = newLevel;
}

int StatManager::GetLevel()
{
	return currentLevel;
}

int StatManager::GetScore()
{
	return score;
}

int StatManager::GetCoin()
{
	return coinCount;
}

int StatManager::GetLife()
{
	return lifeCount;
}