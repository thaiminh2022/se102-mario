#pragma once
#include "Vector2.h"
#include <vector>

using std::vector;

struct SceneEntityData
{
	Vector2Int playerStarts;
	vector<Vector2Int> goombaStarts;
	vector<Vector2Int> questionBlocks;
	vector<Vector2Int> emptyBlocks;
	vector<Vector2Int> coins;
};
