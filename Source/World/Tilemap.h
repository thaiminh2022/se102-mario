#pragma once
#include "Rect.h"
#include "Tile.h"
#include "TilemapConfig.h"
#include "Vector2.h"
#include <memory>
#include <vector>
using std::vector;
using std::unique_ptr;

class Tilemap

{
	unique_ptr<TilemapConfig> config;

public:

	explicit Tilemap(unique_ptr<TilemapConfig> conf, const int forWorld = 0)
		: config(std::move(conf))
	{
	}
	void Render() const;
	TilemapConfig* GetConfig() const { return config.get(); }

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
	int GetRenderIndex() { return -1; }


	void GetPotentialCollidableCells(const RectF& bound, vector<CollisionTile*>& outCells) const;
};

