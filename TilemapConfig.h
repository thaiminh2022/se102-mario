#pragma once
#include "SceneEntityData.h"
#include "Tile.h"
#include <vector>
#include "Color.h"

enum class BiomeType : std::uint8_t
{
	Overworld,
	Underground,
	Castle,
	UnderWater
};



struct TilemapConfig
{
	SceneEntityData entityData;

	int worldWidth;
	int worldHeight;

	int tileWidth;
	int tileHeight;

	vector<RenderLayer> renderLayers;
	CollisionLayer collisionLayer;
	Optional<Color> backgroundColor;
	BiomeType biome;

	TilemapConfig(SceneEntityData entityData, int worldWidth, int worldHeight, int tileWidth, int tileHeight,
	              const vector<RenderLayer>& renderLayers, CollisionLayer collisionLayer,
	              const Optional<Color>& backgroundColor, BiomeType biomeType)
		: entityData(std::move(entityData)),
		  worldWidth(worldWidth),
		  worldHeight(worldHeight),
		  tileWidth(tileWidth),
		  tileHeight(tileHeight),
		  renderLayers(renderLayers),
		  collisionLayer(std::move(collisionLayer)),
		  backgroundColor(backgroundColor), biome(biomeType)
	{
	}
};
