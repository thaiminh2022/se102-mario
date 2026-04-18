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
			// 4. Retrieve the tile from your 1D or 2D array.
			// Replace this with however your Tilemap stores its tiles.
			CollisionTile* tile = config->collisionLayer.GetCell(col, row);

			// 5. If the tile exists and is solid/collidable, add it to our list.
			// (If your array stores nullptrs for empty air tiles, check for that).

			if (tile != nullptr)
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