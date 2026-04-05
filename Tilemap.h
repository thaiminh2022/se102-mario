#pragma once
#include "Rect.h"
#include "Tile.h"
#include "TilemapConfig.h"
#include "Vector2.h"
#include <vector>
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
	TilemapConfig* GetConfig() const { return config; }

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

