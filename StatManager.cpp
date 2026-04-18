#include "StatManager.h"

int StatManager::coinCount = 0;
int StatManager::score = 0;
int StatManager::lifeCount = 0;

void StatManager::AddScore(int score)
{
	StatManager::score += score;
}

void StatManager::AddCoin()
{
	StatManager::coinCount++;
}

void StatManager::AddLife()
{
	StatManager::lifeCount++;
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
