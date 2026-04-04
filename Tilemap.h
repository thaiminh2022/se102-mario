#pragma once
#include <vector>
#include "Rect.h"
#include "Tile.h"
#include "Vector2.h"
using std::vector;

struct TilemapConfig
{
	int playerStartX;
	int playerStartY;


	int worldWidth;
	int worldHeight;

	int tileWidth;
	int tileHeight;

	vector<RenderLayer> renderLayers;
	CollisionLayer collisionLayer;
};

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
		return Vector2Int(config->playerStartX, config->playerStartY);
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

