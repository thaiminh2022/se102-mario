#include "Tilemap.h"

#include "Game.h"
#include "Textures.h"


void Tilemap::Render() const
{
	const auto g = Game::GetInstance();
	const auto cam = g->GetCamera();

	for (const auto& tLayer : config->renderLayers)
	{
		const auto tex = Textures::GetInstance()->Get(tLayer.textureID);
		for (const auto& t :  tLayer.tiles)
		{
			const auto b = t.GetBounds();
			auto pb = t.GetTextureBounds();

			if (!cam->IsInView(
				static_cast<float>(b.left), 
				static_cast<float>(b.top), 
				static_cast<float>(b.right), 
				static_cast<float>(b.bottom))
				)
			{
				continue;
			}

			float renderX, renderY;
			cam->WorldToScreen(static_cast<float>(t.worldX), static_cast<float>(t.worldY), renderX, renderY);
			g->Draw(round(renderX), round(renderY), tex, &pb);
		}
		
	}
}

void Tilemap::GetPlayerStartPosition(int& x, int& y) const
{
	x = config->playerStartX;
	y = config->playerStartY;
}

int Tilemap::GetWidth() const
{
	return config->worldWidth;
}

int Tilemap::GetHeight() const
{
	return config->worldHeight;
}

void Tilemap::GetPotentialCollidableCells(const RectF& bound, vector<Rect>& outCells) const
{
	outCells.clear();


	
}