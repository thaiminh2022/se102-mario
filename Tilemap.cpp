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
		int left = t->x;
		int top = t->y;
		int right = t->x + t->width;
		int bottom = t->y + t->height;
		
		

		if (!cam->IsInView(left, top, right, bottom))
		{
			continue;
		}

		float renderX, renderY;
		cam->WorldToScreen(t->x, t->y, renderX, renderY);
		g->Draw(round(renderX), round(renderY), tex, t->px, t->py, t->px + t->width, t->py + t->height);
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
