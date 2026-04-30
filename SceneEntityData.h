#pragma once
#include <string>

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

struct BrickBlockData
{
	Vector2Int position;
	BlockDropType dropType;
	bool isHidden;
};

struct FlagPoleData
{
	Rect zone;
	Vector2Int moveToPosition;
	vector<Vector2Int> fireworkPositions;
};

struct BridgeData
{
	Rect zone;
	Vector2Int axePosition;
};

struct ReturnPipeData
{
	Vector2Int returnDirection;
	Rect returnRect;
	Vector2Int moveTo;
};
struct PipeData
{
	Rect zone;

	Optional<int> nextLevelToLoad;
	Optional<ReturnPipeData> returnPipeData;

	bool isReturnPipe;
	Vector2Int enterDirection;
	Vector2Int moveTo;

	static Vector2Int GetDirection(const std::string& dir)
	{

		if (dir == "Up")
		{
			return Vector2Int::Up();
		}
		if (dir == "Down")
		{
			return Vector2Int::Down();
		}
		if (dir == "Left")
		{
			return Vector2Int::Left();
		}
		if (dir == "Right")
		{
			return Vector2Int::Right();
		}
		return Vector2Int{};
	}
};



struct SceneEntityData
{
	Optional<BridgeData> bridge;
	Optional<int> backgroundMusicID;
	Vector2Int playerStarts;
	
	vector<PipeData> pipes;
	Optional<FlagPoleData> flagPole;
	
	Optional<Vector2Int> bowserStart;
	Optional<Vector2Int> toadStart;


	vector<Vector2Int> goombaStarts;
	vector<Vector2Int> koopaStarts;
	vector<Vector2Int> WingedKoopaStarts;
	vector<QuestionBlockData> questionBlocks;
	vector<BrickBlockData> brickBlocks;
	vector<Vector2Int> coins;
	vector<NextLevelData> nextLevelsData;
	vector<Vector2Int> fireballTraps;
};

