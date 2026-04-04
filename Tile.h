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
		return Rect::FromXYWH(srcX, srcY, width, height);
	}
};

struct RenderLayer
{
	int textureID;
	int tileWidth, tileHeight;
	int cellWidth, cellHeight;
	vector<RenderTile> tiles;


};

enum CollisionTileType : std::uint8_t
{
	None = 0,
	Ground = 1,
	OneWay = 2,
};

struct CollisionLayer
{
	int tileWidth, tileHeight; // in pixel
	int cWidth, cHeight; // in cells

	vector<CollisionTileType> cells;

	CollisionTileType GetCell(int cx, int cy) const
	{
		if (cx < 0 || cx >= cWidth || cy < 0 || cy >= cHeight)
			return None;

		return cells[(cy * cWidth) + cx];
	}

	bool IsBlockingCell(int cx, int cy) const
	{
		return GetCell(cx, cy) == Ground;
	}

	bool IsOneWayCell(int cx, int cy) const
	{
		return GetCell(cx, cy) == OneWay;
	}

	Rect GetCellBounds(int cx, int cy) const
	{
		int x = cx * tileWidth;
		int y = cy * tileHeight;

		return Rect::FromXYWH(x, y, tileWidth, tileHeight);
	}

};
