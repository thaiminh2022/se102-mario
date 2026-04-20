#pragma once
class StatManager
{
	static int coinCount;
	static int score;
	static int lifeCount;
	static int currentLevel;
public:
	static void AddScore(int addingScore);
	static void AddCoin();
	static void AddLife();
	static void SetLevel(int newLevel);
	static int GetLevel();
	static int GetScore();
	static int GetCoin();
	static int GetLife();
};

