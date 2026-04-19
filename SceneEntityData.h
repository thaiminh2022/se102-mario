#pragma once
#include "Vector2.h"
#include <vector>

#include "OptionalType.h"
#include "Rect.h"	

using std::vector;

struct NextLevelData
{
	Rect zone;
	int levelToLoad;
};
enum class BlockDropType: std::uint8_t
{
	None,
	Coin,
	Starman,
	JewDestroyer,
};


struct QuestionBlockData
{
	Vector2Int position;
	BlockDropType dropType;
};

struct BrickBlocData
{
	Vector2Int position;
	BlockDropType dropType;
};




struct SceneEntityData
{
	Optional<int> backgroundMusicID;
	Vector2Int playerStarts;
	vector<Vector2Int> goombaStarts;
	vector<Vector2Int> koopaStarts;
	vector<Vector2Int> flyingKoopaStarts;
	vector<QuestionBlockData> questionBlocks;
	vector<BrickBlocData> brickBlocks;
	vector<Vector2Int> coins;
	vector<NextLevelData> nextLevelsData;
};

