#include "Tilemap.h"

#include <algorithm>
#include <cmath>

#include "Game.h"
#include "Textures.h"


void Tilemap::Render() const
{
	const auto g = Game::GetInstance();
	const auto cam = g->GetCamera();
	const auto camLeft = cam->GetX();
	const auto camTop = cam->GetY();
	const auto camRight = camLeft + g->GetBackBufferWidth();
	const auto camBottom = camTop + g->GetBackBufferHeight();

	for (const auto& tLayer : config->renderLayers)
	{
		const auto tex = Textures::GetInstance()->Get(tLayer.textureID);

		int startCol = static_cast<int>(std::floor(camLeft / tLayer.tileWidth));
		int endCol = static_cast<int>(std::floor((camRight - 1) / tLayer.tileWidth));
		int startRow = static_cast<int>(std::floor(camTop / tLayer.tileHeight));
		int endRow = static_cast<int>(std::floor((camBottom - 1) / tLayer.tileHeight));

		startCol = max(0, startCol);
		endCol = min(tLayer.cellWidth - 1, endCol);
		startRow = max(0, startRow);
		endRow = min(tLayer.cellHeight - 1, endRow);

		for (int row = startRow; row <= endRow; ++row)
		{
			for (int col = startCol; col <= endCol; ++col)
			{
				const auto t = tLayer.GetCell(col, row);
				if (t == nullptr)
				{
					continue;
				}

				auto pb = t->GetTextureBounds();

				float renderX, renderY;
				cam->WorldToScreen(static_cast<float>(t->worldX), static_cast<float>(t->worldY), renderX, renderY);
				g->Draw(round(renderX), round(renderY), tex, &pb);
			}
		}
	}
}
void Tilemap::GetPotentialCollidableCells(const RectF& bound, vector<CollisionTile*>& outCells) const
{
	outCells.clear();

	int startCol = static_cast<int>(std::floor(bound.left / config->tileWidth));
	int endCol = static_cast<int>(std::floor(bound.right /  config->tileWidth));
	int startRow = static_cast<int>(std::floor(bound.top /  config->tileHeight));
	int endRow = static_cast<int>(std::floor(bound.bottom / config->tileHeight));

	startCol = max(0, startCol);
	endCol = min(config->collisionLayer.cWidth - 1, endCol);
	startRow = max(0, startRow);
	endRow = min(config->collisionLayer.cHeight - 1, endRow);

	//Game::GetInstance()->DrawDebugRectRaw(bound, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.3f));


	// 3. Loop through the overlapping grid section
	for (int row = startRow; row <= endRow; ++row)
	{
		for (int col = startCol; col <= endCol; ++col)
		{
			CollisionTile* tile = config->collisionLayer.GetCell(col, row);

			if (tile != nullptr && tile->type != CollisionTileType::None)
			{
				outCells.push_back(tile);
			}
		}
	}

	/* // Debug: give all the tiles to collision check
	for (auto& tile: config->collisionLayer.cells)
	{
		if (tile.type == CollisionTileType::Ground)
		{
			outCells.push_back(&tile);
		}
	}*/
	
}
