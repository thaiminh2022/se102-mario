#pragma once
class StatManager
{
private:
	int coinCount;
	int score;
	int lifeCount;

	static StatManager* _instance;
	StatManager();
public:
	static StatManager* GetInstance();
	void AddScore(int addingScore);
	void AddCoin(int addingCoin);
	void AddLife(int addingLife);
	int GetScore();
	int GetCoin();
	int GetLife();

	void Reset();
};

