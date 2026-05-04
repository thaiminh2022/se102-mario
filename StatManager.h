#pragma once
class StatManager
{
private:
	int coinCount;
	int score;
	int lifeCount;
	int currentLevel;
	int enemyKillScoreList[10];
	int shellKillScoreList[7];
	int shellKickScoreList[3];

	static StatManager* _instance;
	StatManager();
public:
	static StatManager* GetInstance();
	void Init();
	void AddScore(int addingScore);
	void AddCoin(int addingCoin);
	void AddLife(int addingLife);
	int GetScore();
	int GetCoin();
	int GetLife();
	void Reset();
	void SetLevel(int newLevel);
	int GetLevel();
	void AddEnemyKillScore(int sequenceCount);
	void AddShellKillScore(int sequenceCount);
	void AddShellKickScore(int sequenceCount);
};

