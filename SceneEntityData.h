#pragma once
#include <string>

#include "Vector2.h"
#include <vector>

#include "Color.h"
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

struct MusicTriggerData
{
	int id;
	Rect zone;
};

struct ClearScreenColorTriggerData
{
	Rect zone;
	Color color;

	ClearScreenColorTriggerData(const Rect zone, const Color color)
	{
		this->zone = zone;
		this->color = color;
	}
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
struct BloopersStartData
{
	Vector2Int lowestLimit;
	Vector2Int highestLimit;

	BloopersStartData(const Vector2Int& lowestLimit, const Vector2Int& highestLimit)
		: lowestLimit(lowestLimit),
		  highestLimit(highestLimit)
	{
	}
};

struct InWaterTriggerData
{
	Rect zone;
	bool inWater;

	explicit InWaterTriggerData(const Rect zone, const bool inWater)
		:zone(zone), inWater(inWater)
	{
	}
};

struct PipeData
{
	Rect zone;

	Optional<int> nextLevelToLoad;
	Optional<ReturnPipeData> returnPipeData;

	bool isReturnPipe;
	Vector2Int enterDirection;
	Vector2Int moveTo;
	Optional<Vector2Int> teleportToPosition;
	bool isTeleportPipe;

	PipeData() = default;

	PipeData(const Rect& zone, const Optional<int>& nextLevelToLoad, const Optional<ReturnPipeData>& returnPipeData,
		bool isReturnPipe, const Vector2Int& enterDirection, const Vector2Int& moveTo,
		const Optional<Vector2Int>& teleportToPosition, bool isTeleportPipe)
		: zone(zone),
		  nextLevelToLoad(nextLevelToLoad),
		  returnPipeData(returnPipeData),
		  isReturnPipe(isReturnPipe),
		  enterDirection(enterDirection),
		  moveTo(moveTo),
		  teleportToPosition(teleportToPosition),
		  isTeleportPipe(isTeleportPipe)
	{
	}


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

struct CheepCheepsData
{
	Vector2Int startPosition;
	bool isRed;

	CheepCheepsData(const Vector2Int& startPosition, bool isRed)
		: startPosition(startPosition),
		  isRed(isRed)
	{
	}
};
struct FireShooterData
{
	Vector2Int position;
	Vector2Int shootDirection;
	FireShooterData(const Vector2Int& position, const Vector2Int& shootDirection)
		: position(position),
		  shootDirection(shootDirection)
	{
	}
};

struct SceneEntityData
{
	Optional<BridgeData> bridge;
	Optional<int> backgroundMusicID;
	vector<MusicTriggerData> musicTriggers;
	Vector2Int playerStarts;
	
	vector<PipeData> pipes;
	Optional<FlagPoleData> flagPole;

	vector<ClearScreenColorTriggerData> clearScreenColorTriggers;
	
	Optional<Vector2Int> bowserStart;
	Optional<Vector2Int> toadStart;

	vector<Vector2Int> goombaStarts;
	vector<Vector2Int> koopaStarts;
	vector<CheepCheepsData> cheepCheeps;
	vector<BloopersStartData> bloopers;
	vector<InWaterTriggerData> waterTriggers;
	vector<Vector2Int> WingedKoopaStarts;
	vector<Vector2Int> bowserStarts;
	vector<QuestionBlockData> questionBlocks;
	vector<BrickBlockData> brickBlocks;
	vector<Vector2Int> coins;
	vector<NextLevelData> nextLevelsData;
	vector<Vector2Int> fireballTraps;
	vector<FireShooterData> fireShooters;
};

