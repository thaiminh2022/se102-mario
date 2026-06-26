#pragma once
#include "AssetIDs.h"
#include "TilemapConfig.h"

inline int ChooseIdBaseOnBiome(const BiomeType biome, 
								const int overworld = -1, 
								const int underground = -1,
                               const int castle = -1,
                               const int underwater = -1)
{
	switch (biome)
	{
	case BiomeType::Overworld:
		return overworld;
	case BiomeType::Underground:
		return underground;
	case BiomeType::Castle:
		return castle;
	case BiomeType::UnderWater:
		return underwater;
	}
	return -1;
}

inline int ChooseBlocksId(const BiomeType biome)
{
	return ChooseIdBaseOnBiome(biome, BLOCKS_OVERWORLD_TEX_ID, BLOCKS_UNDERGROUND_TEX_ID, BLOCKS_CASTLE_TEX_ID, BLOCKS_UNDERWATER_TEX_ID);
}
inline int ChooseItemsId(const BiomeType biome)
{
	return ChooseIdBaseOnBiome(biome, OVERWORLD_ITEMS_TEX_ID, UNDERGROUND_ITEMS_TEX_ID, CASTLE_ITEMS_TEX_ID, UNDERWATER_ITEMS_TEX_ID);
}
inline int ChooseEnemyId(const BiomeType biome)
{
	return ChooseIdBaseOnBiome(biome, OVERWORLD_ENEMY_TEX_ID, UNDERGROUND_ENEMY_TEX_ID, CASTLE_ENEMY_TEX_ID, UNDERWATER_ENEMY_TEX_ID);
}

inline int ChooseTilesetId(const BiomeType biome)
{
	return ChooseIdBaseOnBiome(biome, -1, -2, -3, -4);
}
