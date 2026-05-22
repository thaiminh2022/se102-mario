#pragma once
#include <vector>

#include "CollisionMatrix.h"
#include "Rect.h"

using std::vector;

struct RenderTile
{
	int worldX, worldY; // in-game world coords (in pixel)
	int srcX, srcY; // texture coords (in pixel)
	int width, height;

	Rect GetBounds() const
	{
		return Rect::FromXYWH(worldX, worldY, width, height);
	}
	Rect GetTextureBounds() const
	{
		return Rect::FromXYWH(srcX, srcY, width - 1, height -1);
	}
};

struct RenderLayer
{
	int textureID;
	int tileWidth, tileHeight;
	int cellWidth, cellHeight;
	vector<RenderTile> tiles;
	vector<int> tileLookup;

	RenderTile* GetCell(int cx, int cy)
	{
		if (cx < 0 || cx >= cellWidth || cy < 0 || cy >= cellHeight)
			return nullptr;

		const int lookupIndex = cy * cellWidth + cx;
		const int tileIndex = tileLookup[lookupIndex];

		if (tileIndex < 0)
			return nullptr;

		return &tiles[tileIndex];
	}

	const RenderTile* GetCell(int cx, int cy) const
	{
		if (cx < 0 || cx >= cellWidth || cy < 0 || cy >= cellHeight)
			return nullptr;

		const int lookupIndex = cy * cellWidth + cx;
		const int tileIndex = tileLookup[lookupIndex];

		if (tileIndex < 0)
			return nullptr;

		return &tiles[tileIndex];
	}

	const RenderTile* GetTileAtWorldPosition(int x, int y) const
	{
		return GetCell(x / tileWidth, y / tileHeight);
	}
};

enum class CollisionTileType : std::uint8_t
{
	None = 0,
	Ground = 1,
	OneWay = 2,
	Death = 3,
};

struct CollisionTile
{
	int worldX, worldY;
	int tileWidth , tileHeight;
	CollisionTileType type;

	Rect GetBounds() const
	{
		return Rect::FromXYWH(worldX, worldY, tileWidth, tileHeight);
	}

	// tiles that will make player back tf off
	bool IsBlocking() const
	{
		return type == CollisionTileType::Ground || type == CollisionTileType::OneWay;
	}
	CollisionMatrixLayer GetCollisionLayer()
	{
		return CollisionMatrixLayer::Ground;
	}

};

struct CollisionLayer
{
	int tileWidth, tileHeight; // in pixel
	int cWidth, cHeight; // in cells

	vector<CollisionTile> cells;

	CollisionTile* GetCell(int cx, int cy)
	{
		if (cx < 0 || cx >= cWidth || cy < 0 || cy >= cHeight)
			return nullptr;
		int index = cy * cWidth + cx;

		return &cells[index];
	}
};
