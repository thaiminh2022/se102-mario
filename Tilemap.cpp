#include "Tilemap.h"

#include "Game.h"
#include "Textures.h"


void Tilemap::Render() const
{
	const auto g = Game::GetInstance();
	const auto tex = Textures::GetInstance()->Get(config->textureID);
	const auto cam = g->GetCamera();
	for (const auto& t : config->tiles)
	{
		auto b = t.GetBounds();
		auto pb = t.GetTextureBounds();

		if (!cam->IsInView(b.left, b.top, b.right, b.bottom))
		{
			continue;
		}

		float renderX, renderY;
		cam->WorldToScreen(t.x, t.y, renderX, renderY);
		g->Draw(round(renderX), round(renderY), tex, &pb);
	}
}

void Tilemap::GetPlayerStartPosition(int& x, int& y) const
{
	x = config->playerStartX;
	y = config->playerStartY;
}

int Tilemap::GetWidth() const
{
	return config->width;
}

int Tilemap::GetHeight() const
{
	return config->height;
}

void Tilemap::GetPotentialColliableTiles(RECT r, vector<Tile*>& outTiles) const
{
	for (auto tile : config->tiles)
	{
		if (!tile.IsCollidable())
			continue;

		auto bounds = tile.GetBounds();
		
		if (bounds.right < r.left || bounds.left > r.right 
			|| bounds.bottom < r.top || bounds.top > r.bottom)
			continue;

		outTiles.push_back(&tile);
	}
}
