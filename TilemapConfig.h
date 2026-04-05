#pragma once
#include "SceneEntityData.h"
#include "Tile.h"
#include <vector>

struct TilemapConfig
{
	SceneEntityData entityData;

	int worldWidth;
	int worldHeight;

	int tileWidth;
	int tileHeight;

	vector<RenderLayer> renderLayers;
	CollisionLayer collisionLayer;

	TilemapConfig(const SceneEntityData& entity_data, int world_width, int world_height, int tile_width,
		int tile_height, const vector<RenderLayer>& render_layers, const CollisionLayer& collision_layer)
		: entityData(entity_data),
		  worldWidth(world_width),
		  worldHeight(world_height),
		  tileWidth(tile_width),
		  tileHeight(tile_height),
		  renderLayers(render_layers),
		  collisionLayer(collision_layer)
	{
	}
};
