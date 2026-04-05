#pragma once
#include <vector>

#include "Rect.h"
#include "Tile.h"
#include "TilemapConfig.h"
#include "Vector2.h"
using std::vector;

class Tilemap
{
	TilemapConfig* config;

public:
	explicit Tilemap(TilemapConfig* conf)
	{
		config = conf;
	}
	void Render() const;
	Vector2Int GetPlayerStartPosition() const
	{
		return config->entityData.playerStarts;
	}
	int GetWidth() const
	{
		return config->worldWidth;
	}
	int GetHeight() const
	{
		return config->worldHeight;
	}
	int GetTileWidth() const { return config->tileWidth; }
	int GetTileHeight() const { return config->tileHeight; }

	void GetPotentialCollidableCells(const RectF& bound, vector<CollisionTile*>& outCells) const;
};

