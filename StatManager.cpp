#include "StatManager.h"

int StatManager::coinCount = 0;
int StatManager::score = 0;
int StatManager::lifeCount = 0;
int StatManager::currentLevel = 1;


void StatManager::AddScore(int addingScore)
{
	StatManager::score += addingScore;
}

void StatManager::AddCoin()
{
	StatManager::coinCount++;
}

void StatManager::AddLife()
{
	StatManager::lifeCount++;
}

void StatManager::SetLevel(int newLevel)
{
	StatManager::currentLevel = newLevel;
}

int StatManager::GetLevel()
{
	return StatManager::currentLevel;
}

int StatManager::GetScore()
{
	return StatManager::score;
}

int StatManager::GetCoin()
{
	return StatManager::coinCount;
}

int StatManager::GetLife()
{
	return StatManager::lifeCount;
}
