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
	float delaySeconds;
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

struct FlagPoleData
{
	Rect zone;
	Vector2Int moveToPosition;
};


struct SceneEntityData
{
	Optional<int> backgroundMusicID;
	Vector2Int playerStarts;
	Optional<FlagPoleData> flagPole;
	vector<Vector2Int> goombaStarts;
	vector<QuestionBlockData> questionBlocks;
	vector<BrickBlocData> brickBlocks;
	vector<Vector2Int> coins;
	vector<NextLevelData> nextLevelsData;
	vector<Vector2Int> fireballTraps;
};

