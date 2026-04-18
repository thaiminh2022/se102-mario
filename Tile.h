#pragma once
#include <vector>

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


};

enum class CollisionTileType : std::uint8_t
{
	None,
	Ground,
	OneWay,
	Death,
	Pipe,
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
		return type == CollisionTileType::Ground || type == CollisionTileType::OneWay || type == CollisionTileType::Pipe;
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
