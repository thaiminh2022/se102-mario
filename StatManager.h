#pragma once
class StatManager
{
	static int coinCount;
	static int score;
	static int lifeCount;
public:
	static void AddScore(int addingScore);
	static void AddCoin();
	static void AddLife();
	static int GetScore();
	static int GetCoin();
	static int GetLife();
};

