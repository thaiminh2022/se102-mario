#include "Tilemap.h"

#include "Game.h"
#include "Textures.h"


void Tilemap::Render() 
{
	const auto g = Game::GetInstance();
	const auto tex = Textures::GetInstance()->Get(textureID);
	for (const auto& t : tiles)
	{
		g->Draw(t->x, t->y, tex, t->px, t->py, t->px + t->width, t->py + t->height);
	}
}
