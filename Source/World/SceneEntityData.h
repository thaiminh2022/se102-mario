#pragma once
#include <string>

#include "Vector2.h"
#include <vector>

#include "Color.h"
#include "FontManager.h"
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

	BridgeData() = default;

	BridgeData(const Rect& zone, const Vector2Int& axePosition)
		: zone(zone), axePosition(axePosition)
	{
	}
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

struct EnterCastleTriggerData
{
	Rect zone;
	vector<Vector2Int> fireworkPositions;
	Optional<Vector2Int> flagMoveTo;
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
struct BowserArenaData
{
	Rect arenaZone;
	BowserArenaData(const Rect& arenaZone)
		: arenaZone(arenaZone)
	{
	}
	BowserArenaData() = default;
};

struct BowserEntityData
{
	Rect container;
	Vector2Int start;


	BowserEntityData(Rect container, Vector2Int start)
	{
		this->container = container;
		this->start = start;
	}
};

struct WorldTextData
{
	Rect zone;
	std::wstring content;
	TextFormat textFormat;
	FontWeight fontWeight;
	bool italic;
};

struct ForceVelocityTriggerData
{
	Rect zone;
	Vector2Int moveTo;
};

struct MovingPlatformData
{
	Rect zone;
	vector<Vector2Int> movingPoints;
	float moveSpeed = 0.0f;
	bool loop = false;
	bool loopPingPong = false;
};

struct SceneEntityData
{
	// entities
	Optional<BridgeData> bridge;
	Vector2Int playerStarts;

	vector<Vector2Int> goombaStarts;
	vector<Vector2Int> koopaStarts;
	vector<CheepCheepsData> cheepCheeps;
	vector<BloopersStartData> bloopers;
	vector<InWaterTriggerData> waterTriggers;
	vector<Vector2Int> WingedKoopaStarts;
	vector<QuestionBlockData> questionBlocks;
	vector<BrickBlockData> brickBlocks;
	vector<Vector2Int> coins;
	vector<NextLevelData> nextLevelsData;
	vector<Vector2Int> fireballTraps;
	vector<FireShooterData> fireShooters;
	vector<MovingPlatformData> movingPlatforms;

	vector<Vector2Int> superLeafStart;
	vector<WorldTextData> worldTextData;
	Optional<BowserEntityData> bowserData;
	
	// game play
	Optional<int> backgroundMusicID;
	vector<PipeData> pipes;
	Optional<FlagPoleData> flagPole;

	// trigger
	vector<MusicTriggerData> musicTriggers;
	vector<ClearScreenColorTriggerData> clearScreenColorTriggers;
	Optional<EnterCastleTriggerData> enterCastleTrigger;
	vector<ForceVelocityTriggerData> forceVelocityTriggers;

};

