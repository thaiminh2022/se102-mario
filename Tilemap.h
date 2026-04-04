#pragma once
#include <vector>
#include "Rect.h"
#include "Tile.h"
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
	void GetPlayerStartPosition(int& x, int& y) const;
	int GetWidth() const;
	int GetHeight() const;
	int GetTileWidth() const { return config->tileWidth; }
	int GetTileHeight() const { return config->tileHeight; }

	void GetPotentialCollidableCells(const RectF& bound, vector<Rect>& outCells) const;

	bool IsBlockingCell(int row, int col) const
	{
		return config->collisionLayer.IsBlockingCell(row, col);
	}

	bool IsOneWayCell(int row, int col) const
	{
		return config->collisionLayer.IsOneWayCell(row, col);
	}

	Rect GetCellBounds(int cx, int cy) const
	{
		return config->collisionLayer.GetCellBounds(cx, cy);
	}
};

