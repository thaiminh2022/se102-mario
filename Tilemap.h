#pragma once
#include <vector>
using std::vector;


struct Tile
{
	int value; // for int grid
	int x, y;
	int px, py;
	int width, height;
};

class Tilemap
{
	int textureID;
	vector<Tile*> tiles;
	
public:
	int playerStartX;
	int playerStartY;

	Tilemap(int textureID, const vector<Tile*>& tiles, int playerStartX, int playerStartY)
	{
		this->textureID = textureID;
		this->tiles = tiles;
		this->playerStartY = playerStartY;
		this->playerStartX = playerStartX;
	}
	void Render();
};

