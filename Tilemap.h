#pragma once
#include <vector>
using std::vector;


struct Tile
{
	int value; // for int grid (collision, oneway platform, dead zone)
	int x, y;
	int px, py;
	int width, height;
};
struct TilemapConfig
{
	int textureID;
	const vector<Tile*> tiles;
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
};

