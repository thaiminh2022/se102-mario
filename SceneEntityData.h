#pragma once
#include "Vector2.h"
#include <vector>
#include "Rect.h"	

using std::vector;

struct NextLevelData
{
	Rect zone;
	int levelToLoad;
};


struct SceneEntityData
{
	Vector2Int playerStarts;
	vector<Vector2Int> goombaStarts;
	vector<Vector2Int> questionBlocks;
	vector<Vector2Int> emptyBlocks;
	vector<Vector2Int> coins;
	vector<NextLevelData> nextLevelsData;
};

