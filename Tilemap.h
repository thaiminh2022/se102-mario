#pragma once
#include <vector>
#include <Windows.h>
using std::vector;

enum TileType
{
	None = 0, // decorative only;
	Ground = 1, // Anything can walk on with full collision
	OnewayPlatform = 2, // Platforms player can jump up to, but not drop down
};
struct Tile
{
	TileType value; // for int grid (collision, oneway platform, dead zone)
	int x, y;
	int px, py;
	int width, height;

	bool IsCollidable() { return value != None; }
	RECT GetBounds() const
	{
		RECT r;
		r.left = x;
		r.top = y;
		r.right = x + width;
		r.bottom = y + height;

		return r;
	}

	RECT GetTextureBounds() const
	{
		RECT r;
		r.left = px;
		r.top = py;
		r.right = px + width;
		r.bottom = py + height;

		return r;
	}
};
struct TilemapConfig
{
	int textureID;
	vector<Tile> tiles;
	int playerStartX;
	int playerStartY;
	int width;
	int height;
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

	const vector<Tile>& GetTiles() const { return  config->tiles; }
	void GetPotentialColliableTiles(RECT r, vector<Tile*>& outTiles) const;

};

