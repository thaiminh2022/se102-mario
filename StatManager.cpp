#include "StatManager.h"
#include "PointPopup.h"
#include "Game.h"

StatManager* StatManager::_instance = nullptr;

StatManager* StatManager::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new StatManager();
	}
	return _instance;
}
void StatManager::Init() {
	// Initialize the score lists for enemy kills and shell kills
	enemyKillScoreList[0] = 100;
	enemyKillScoreList[1] = 200;
	enemyKillScoreList[2] = 400;
	enemyKillScoreList[3] = 500;
	enemyKillScoreList[4] = 800;
	enemyKillScoreList[5] = 1000;
	enemyKillScoreList[6] = 2000;
	enemyKillScoreList[7] = 4000;
	enemyKillScoreList[8] = 5000;
	enemyKillScoreList[9] = 8000;

	shellKillScoreList[0] = 500;
	shellKillScoreList[1] = 800;
	shellKillScoreList[2] = 1000;
	shellKillScoreList[3] = 2000;
	shellKillScoreList[4] = 4000;
	shellKillScoreList[5] = 5000;
	shellKillScoreList[6] = 8000;

	shellKickScoreList[0] = 400;
	shellKickScoreList[1] = 500;
	shellKickScoreList[2] = 800;
}
StatManager::StatManager()
{
	Init();
	Reset(); // Set initial values when created
}

void StatManager::Reset()
{
	coinCount = 0;
	score = 0;
	lifeCount = 3; // Starting lives
}

void StatManager::AddScore(int addingScore, Vector2 pos)
{
	score += addingScore;
	PointPopup* popup = new PointPopup(pos, addingScore);
	auto ctx = dynamic_cast<PlayableScene*>(Game::GetInstance()->GetCurrentScene());
	if (ctx != nullptr) {
		ctx->AddObject(popup); //currently, will think of a better way to add popup later
	}
}

void StatManager::AddCoin(int addingCoin)
{
	coinCount += addingCoin;
}

void StatManager::AddLife(int addingLife, Vector2 pos)
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
void StatManager::AddEnemyKillScore(int sequenceCount, Vector2 pos)
{
	if (sequenceCount < 0)
		return;
	else if (sequenceCount >= 0 && sequenceCount < 10) {
		int score = enemyKillScoreList[sequenceCount];
		AddScore(score, pos);
	}
	else
		AddLife(1, pos);//currently no popup for 1up, will change later
}

void StatManager::AddShellKillScore(int sequenceCount, Vector2 pos)
{
	if (sequenceCount < 0)
		return;
	else if (sequenceCount >= 0 && sequenceCount < 7) {
		int score = shellKillScoreList[sequenceCount];
		AddScore(score, pos);
	}
	else
		AddLife(1, pos);
}

void StatManager::AddShellKickScore(int sequenceCount, Vector2 pos)
{
	if (sequenceCount < 0)
		return;
	else if (sequenceCount >= 0 && sequenceCount < 3) {
		int score = shellKickScoreList[sequenceCount];
		AddScore(score, pos);
	}
	else {
		int score = shellKickScoreList[2];
		AddScore(score, pos);
	}
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