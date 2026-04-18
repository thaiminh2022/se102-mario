#include "Debug.h"
#include "LevelLoader.h"
#include "Textures.h"

#include <fstream>
#include <string>
#include <vector>

#include "AudioManager.h"
#include "Game.h"
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
constexpr auto LEVEL_0_TILESET = L"Assets/Sprites/AssetsReference/fulltileset.png";
constexpr auto LEVEL_1_TILESET = L"Assets/Sprites/AssetsReference/fulltileset.png";

const string WORLD_PATH = "world_map.ldtk";

// LAYER
const string COLLISION_LAYER = "Collision";
const string BACKGROUND_LAYER = "Background";
const string DYNAMIC_LAYER = "Dynamic";

// ENTITY
const string PLAYER_START = "PlayerStart";
const string GOOMBA_START= "GoombaStart";
const string KOOPA_START = "KoopaStart";
const string QUESTION_BLOCK= "QuestionBlock";
const string BRICK_BLOCK= "EmptyBrickBlock";
const string COIN = "Coin";
const string NEXT_LEVEL_ZONE = "NextLevel";
const string BACKGROUND_MUSIC = "BackgroundMusic";
RenderLayer LevelLoader::ParseCollisionRenderLayer(const vector<LayerInstance>& v)
{
	auto layerData = GetLayerWithIdentifier(v, COLLISION_LAYER);
	RenderLayer renderLayer;
	int tID = -1;

	auto texturePath = layerData->tilesetRelPath;
	wstring path = LEVEL_0_TILESET;
	if (texturePath.hasValue)
		path = wstring(texturePath.value.begin(), texturePath.value.end());

	if (!Textures::GetInstance()->HaveTextureWithPath(path, tID))
		DebugOut(L"[ERROR] Cannot find tileset for collision layer");

	renderLayer.textureID = tID;

	// Collision dùng autoLayerTiles vì có RULES
	for (auto& t : layerData->autoLayerTiles)
	{
		RenderTile tile;
		tile.worldX = t.px[0];
		tile.worldY = t.px[1];
		tile.srcX = t.src[0];
		tile.srcY = t.src[1];
		tile.width = 16;
		tile.height = 16;
		renderLayer.tiles.push_back(tile);
	}
const string FIREBALL_TRAP = "FireballTrap";

	return renderLayer;
}
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
	t->Add(-2, LEVEL_1_TILESET); 


	ifstream f(WORLD_PATH);
	const auto data = json::parse(f);
	worldMap.Set(data.get<WorldMap>());

	for (auto i = 0; i < worldMap.value.levels.size(); i++)
	{
		if (Game::GetInstance()->HaveSceneWithID(i))
			continue;

		const auto scene = new PlayableScene(i);
		Game::GetInstance()->AddScene(-i, scene);
	}


	f.close();
}


Tilemap* LevelLoader::ParseLevel(int level)
{
	if (!worldMap.hasValue)
		Init();

	const auto& map = worldMap.value;
	if (level >= map.levels.size())
		return nullptr;

	const auto& levelData = map.levels[level];
	if (!levelData.layerInstances.hasValue)
		return nullptr;

	const auto& layersValue = levelData.layerInstances.value;

	vector<RenderLayer> renderLayers;
	const auto col = ParseCollisionLayer(layersValue);
	const auto r1 = ParseBackgroundLayer(layersValue);
	const auto r2 = ParseCollisionRenderLayer(layersValue);
	auto entitiesData = ParseEntityLayer(level, layersValue);

	renderLayers.push_back(r1);
	renderLayers.push_back(r2); 

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

	auto texturePath = layerData->tilesetRelPath;
	wstring path = LEVEL_0_TILESET;
	
	if (texturePath.hasValue)
	{
		path = wstring(texturePath.value.begin(), texturePath.value.end());
	}

	if (!Textures::GetInstance()->HaveTextureWithPath(path, tID))
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
		string blockDrop;
		blockDrop = g->fieldInstances[0].value.get<string>();
		auto blockDropValue = BlockDropType::None;

		if (blockDrop == "Coin")
		{
			blockDropValue = BlockDropType::Coin;

		}else if (blockDrop == "JewDestroyer")
		{
			blockDropValue = BlockDropType::JewDestroyer;

		}else if (blockDrop == "Starman")
		{
			blockDropValue = BlockDropType::Starman;

		}else
		{
			DebugOut(L"[Error] block drop value not exists, default to none");

		}


		auto data = QuestionBlockData{
			Vector2Int(g->px[0], g->px[1]),
			blockDropValue,
		};

		sceneEntities.questionBlocks.push_back(data);
	}

	// Brick block
	const auto eBlocks = GetEntityDataWithIdentifier(entities, BRICK_BLOCK);
	for (const auto&g : eBlocks)
	{
		auto blockDrop = g->fieldInstances[0].value.get<string>();
		auto blockDropValue = BlockDropType::None;
		if (blockDrop == "Coin")
		{
			blockDropValue = BlockDropType::Coin;
		}
		else if (blockDrop == "JewDestroyer")
		{
			blockDropValue = BlockDropType::JewDestroyer;
		}
		else if (blockDrop == "Starman")
		{
			blockDropValue = BlockDropType::Starman;
		}
		else
		{
			DebugOut(L"[Error] block drop value not exists, default to none");
		}


		auto data = BrickBlocData{
			Vector2Int(g->px[0], g->px[1]),
			blockDropValue,
		};

		sceneEntities.brickBlocks.push_back(data);
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
			-value 	// playable levels are stored as 0 or negative values
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
	// fireball trap
	const auto traps = GetEntityDataWithIdentifier(entities, FIREBALL_TRAP);
	for (const auto& g : traps)
	{
		sceneEntities.fireballTraps.emplace_back(g->px[0], g->px[1]);
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

