#include "Debug.h"
#include "LevelLoader.h"
#include "Textures.h"

#include <fstream>
#include <string>
#include <vector>

#include "AudioManager.h"
#include "LdtkParser.h"	
#include "nloahmann/json.hpp"
#include "SceneEntityData.h"
#include "Tile.h"
#include "Tilemap.h"
#include "TilemapConfig.h"
#include "Vector2.h"

using json = nlohmann::json;
using namespace std;
LevelLoader *LevelLoader::_instance = nullptr;



// PATH
constexpr auto LEVEL_0_TILESET = L"Assets/Sprites/ground_and_stone_overworld.png";
const string WORLD_PATH = "world_map.ldtk";

// LAYER
const string COLLISION_LAYER = "Collision";
const string BACKGROUND_LAYER = "Background";
const string DYNAMIC_LAYER = "Dynamic";

// ENTITY
const string PLAYER_START = "PlayerStart";
const string GOOMBA_START= "GoombaStart";
const string QUESTION_BLOCK= "QuestionBlock";
const string EMPTY_BRICK_BLOCK= "EmptyBrickBlock";
const string COIN = "Coin";
const string NEXT_LEVEL_ZONE = "NextLevel";
const string BACKGROUND_MUSIC = "BackgroundMusic";

/// Return the tilemap object for [level]. Value will be cached if new
Tilemap* LevelLoader::GetTilemapForLevel(const int level)
{
	if (tilemaps.find(level) == tilemaps.end())
	{
		// did not find shit, need parse
		auto tilemap = ParseLevel(level);

		if (tilemap == nullptr)
			return nullptr;
		tilemaps[level] = tilemap;
	}

	return tilemaps[level];
}

/// Parse the [worldMap.ldtk] json file and store in memory
void LevelLoader::Init()
{
	const auto t = Textures::GetInstance();
	t->Add(-1, LEVEL_0_TILESET);

	ifstream f(WORLD_PATH);
	const auto data = json::parse(f);
	worldMap.Set(data.get<WorldMap>());

	f.close();
}


Tilemap *LevelLoader::ParseLevel(int level)
{
	if (!worldMap.hasValue)
	{
		// what happened lol, though you got init innit?
		Init();
	}

	const auto& map = worldMap.value;
	if (level > map.levels.size())
		return nullptr;
	
	const auto& levelData = map.levels[level];
	if (!levelData.layerInstances.hasValue)
	{
		return nullptr;
	}
	const auto& layersValue = levelData.layerInstances.value;
	/// -----
	vector<RenderLayer> renderLayers;
	const auto col = ParseCollisionLayer(layersValue);
	const auto r1 = ParseBackgroundLayer(layersValue);
	auto entitiesData = ParseEntityLayer(level, layersValue);

	renderLayers.push_back(r1);

	
	auto config = new TilemapConfig(
		entitiesData, 
		levelData.pxWid, 
		levelData.pxHei, 
		col.tileWidth, 
		col.tileHeight, 
		renderLayers, 
		col);

	const auto tilemap = new Tilemap(config);
	return tilemap;
}
const LayerInstance* LevelLoader::GetLayerWithIdentifier(
	const vector<LayerInstance>& v,
	const string& identifier)
{
	for (const auto& layer : v)
	{
		if (layer.identifier == identifier)
		{
			return &layer;
		}
	}
	return nullptr;
}

CollisionLayer LevelLoader::ParseCollisionLayer(const vector<LayerInstance>& v)
{
	// collision layer is store as an int grid
	CollisionLayer col;
	auto layerData = GetLayerWithIdentifier(v, COLLISION_LAYER);
	col.tileWidth = 16;
	col.tileHeight = 16;
	col.cWidth = layerData->cWid;
	col.cHeight = layerData->cHei;

	// parse collision
	int i = 0;
	for (const auto value: layerData->intGridCsv)
	{
		CollisionTile t;
		t.tileWidth = 16;
		t.tileHeight = 16;

		t.worldX = i % layerData->cWid * 16;
		t.worldY = i / layerData->cWid * 16;
		
		// danger, please make sure this match ldtk!
		t.type = static_cast<CollisionTileType>(value);

		col.cells.push_back(t);
		i++;
	}
	return col;
}



RenderLayer LevelLoader::ParseBackgroundLayer(const vector<LayerInstance>& v)
{
	auto layerData = GetLayerWithIdentifier(v, BACKGROUND_LAYER);
	auto renderLayer = RenderLayer();
	int tID = -1;

	if (!Textures::GetInstance()->HaveTextureWithPath(LEVEL_0_TILESET, tID))
	{
		DebugOut(L"[ERROR] Cannot fine tileset, resolve to default: -1");
	}
	renderLayer.textureID = tID;

	for (auto l : layerData->gridTiles)
	{
		RenderTile t;
		t.worldX = l.px[0];
		t.worldY= l.px[1];
		t.srcX = l.src[0];
		t.srcY = l.src[1];

		t.width = 16;
		t.height = 16;
		renderLayer.tiles.push_back(t);
	}
	return renderLayer;
}

SceneEntityData LevelLoader::ParseEntityLayer(const int level, const vector<LayerInstance>& v)
{
	SceneEntityData sceneEntities;
	
	auto layer = GetLayerWithIdentifier(v, DYNAMIC_LAYER);
	auto entities = layer->entityInstances;

	if (entities.empty())
		return sceneEntities;

	
	// player start - always have 1
	const auto pStart = GetEntityDataWithIdentifier(entities, PLAYER_START)[0];
	sceneEntities.playerStarts = Vector2Int(pStart->px[0], pStart->px[1]);


	// NOTE: emplace_back is push_back but takes in a constructor, so no temp object creation is needed
	// Goomba
	const auto goombas = GetEntityDataWithIdentifier(entities, GOOMBA_START);
	for (const auto& g: goombas)
	{
		sceneEntities.goombaStarts.emplace_back(g->px[0], g->px[1]);
	}

	// Question
	const auto qBlocks = GetEntityDataWithIdentifier(entities, QUESTION_BLOCK);
	for (const auto& g : qBlocks)
	{
		sceneEntities.questionBlocks.emplace_back(g->px[0], g->px[1]);
	}

	// Empty
	const auto eBlocks = GetEntityDataWithIdentifier(entities, EMPTY_BRICK_BLOCK);
	for (const auto&g : eBlocks)
	{
		sceneEntities.emptyBlocks.emplace_back(g->px[0], g->px[1]);
	}

	// Coins
	const auto coins = GetEntityDataWithIdentifier(entities, COIN);
	for (const auto& g : coins)
	{
		sceneEntities.coins.emplace_back(g->px[0], g->px[1]);
	}

	// Next level zone
	const auto nextLevels = GetEntityDataWithIdentifier(entities, NEXT_LEVEL_ZONE);
	for (const auto& g : nextLevels)
	{
		auto zone = Rect::FromXYWH(g->px[0], g->px[1], g->width, g->height);
		auto value = g->fieldInstances[0].value.get<int>(); // just hard code it for now, since there's only 1 value

		NextLevelData data{
			zone, 
			value
		};

		sceneEntities.nextLevelsData.push_back(data);
	}

	// Background music - there should be only once, as enforced in ldtk
	const auto bgMusic = GetEntityDataWithIdentifier(entities, BACKGROUND_MUSIC);
	
	if (!bgMusic.empty())
	{
		const auto data = bgMusic[0]->fieldInstances[0];
		const auto musicPath = data.value.get<string>();
		const auto utf16String = wstring(musicPath.begin(), musicPath.end());

		auto id = AudioManager::GetInstance()->GetIdForWAVFile(utf16String.c_str());

		if (id.hasValue)
		{
			sceneEntities.backgroundMusicID.Set(id.value); // load the music here too
		}else
		{
			AudioManager::GetInstance()->LoadWAV(-1 - level, utf16String.c_str());
			sceneEntities.backgroundMusicID.Set(-1 - level);
		}

	}
	return sceneEntities;
}

vector<EntityInstance*> LevelLoader::GetEntityDataWithIdentifier(vector<EntityInstance>& v, const string& iden)
{
	vector<EntityInstance*> instances;
	for (auto& e : v)
	{
		if (e.identifier == iden)
		{
			instances.push_back(&e);
		}
	}
	return instances;
}

