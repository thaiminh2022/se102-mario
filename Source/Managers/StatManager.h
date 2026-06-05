#pragma once
#include "Vector2.h"
class StatManager
{
private:
	int coinCount;
	int score;
	int lifeCount;
	int currentLevel;

	int lastCoinCount;
	int lastScoreCount;
	int lastLevel;


	int enemyKillScoreList[10];
	int shellKillScoreList[7];
	int shellKickScoreList[3];

	static StatManager* _instance;
	StatManager();
public:
	static StatManager* GetInstance();
	void Init();
	void AddScore(int addingScore, Vector2 pos);
	void AddCoin(int addingCoin);
	void AddLife(int addingLife, Vector2 pos);
	int GetScore() const;
	int GetCoin() const;
	int GetLife() const;
	void Reset();
	void SetLevel(int newLevel);
	int GetLevel() const;
	void AddEnemyKillScore(int sequenceCount, Vector2 pos);
	void AddShellKillScore(int sequenceCount, Vector2 pos);
	void AddShellKickScore(int sequenceCount, Vector2 pos);

	void StartRecordingValues();
	void CommitValues();
	void UndoValues();
};

