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




const string WORLD_PATH = "world_map-new.ldtk";

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
const string FIREBALL_TRAP = "FireballTrap";
const string FLAG_POLE = "FlagPole";
const string BOWSER_START = "FlagPole";
const string BRIDGE = "Bridge";
const string TOAD_START = "FlagPole";



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
	t->Add(-1, L"Assets/Sprites/tileset_overworld.png");
	t->Add(-2, L"Assets/Sprites/tileset_underground.png"); 
	t->Add(-3, L"Assets/Sprites/tileset_castle.png");

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


Tilemap *LevelLoader::ParseLevel(int level)
{
	if (!worldMap.hasValue)
	{
		// what happened lol, though you got init innit?
		Init();
	}

	const auto& map = worldMap.value;
	if (level >= map.levels.size())
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

	
	Optional<Color> bgColor;
	if (!levelData.bgColor.empty())
	{
		bgColor.Set(Color(levelData.bgColor));
	}

	const auto biomeJson = GetFieldValueWithIdentifier(levelData.fieldInstances, "Biome");
	auto levelBiome = BiomeType::Overworld;
	if (biomeJson.hasValue)
	{
		auto biome = biomeJson.value.get<string>();
		if (biome == "Overworld")
		{
			levelBiome = BiomeType::Overworld;
		}else if (biome == "Underground")
		{
			levelBiome = BiomeType::Underground;
		}else if (biome == "Castle")
		{
			levelBiome = BiomeType::Castle;
		}
	}


	const auto config = new TilemapConfig(
		entitiesData,
		levelData.pxWid,
		levelData.pxHei,
		col.tileWidth,
		col.tileHeight,
		renderLayers,
		col,
		bgColor,
		levelBiome
	);

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
	wstring path;
	
	if (texturePath.hasValue)
	{
		path = wstring(texturePath.value.begin(), texturePath.value.end());
	}

	if (!Textures::GetInstance()->HaveTextureWithPath(path, tID))
	{
		DebugOut(L"[ERROR] Cannot fine tileset");
		throw;
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

void LevelLoader::ParsePlayerStart(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities)
{
	// player start - always have 1
	const auto pStart = GetEntityDataWithIdentifier(entities, PLAYER_START)[0];
	sceneEntities.playerStarts = Vector2Int(pStart->px[0], pStart->px[1]);
}

void LevelLoader::ParseGoombas(SceneEntityData& sceneEntities, vector<EntityInstance> entities)
{
	// Goomba
	const auto goombas = GetEntityDataWithIdentifier(entities, GOOMBA_START);
	for (const auto& g: goombas)
	{
		sceneEntities.goombaStarts.emplace_back(g->px[0], g->px[1]);
	}
}

void LevelLoader::ParseKoopas(SceneEntityData& sceneEntities, vector<EntityInstance> entities)
{
	const auto koopas = GetEntityDataWithIdentifier(entities, KOOPA_START);
	for (const auto& g : koopas)
	{
		sceneEntities.koopaStarts.emplace_back(g->px[0], g->px[1]);
	}
}

void LevelLoader::ParseBowsers(SceneEntityData& sceneEntities, vector<EntityInstance> entities)
{
	const auto bowserStart = GetEntityDataWithIdentifier(entities, BOWSER_START);
	if (!bowserStart.empty())
	{
		const auto s = bowserStart[0]; // only 1 per level;
		sceneEntities.bowserStart.Set(Vector2Int(s->px[0], s->px[1]));
	}
}

void LevelLoader::ParseToad(SceneEntityData& sceneEntities, vector<EntityInstance> entities)
{
	const auto toadStart = GetEntityDataWithIdentifier(entities, TOAD_START);
	if (!toadStart.empty())
	{
		const auto s = toadStart[0]; // only 1 per level;
		sceneEntities.toadStart.Set(Vector2Int(s->px[0], s->px[1]));
	}
}

void LevelLoader::ParseBridge(SceneEntityData& sceneEntities, vector<EntityInstance> entities)
{
	const auto bridge = GetEntityDataWithIdentifier(entities, BRIDGE);
	if (!bridge.empty())
	{
		const auto s = bridge[0]; // only 1 per level;
		auto axePosJson = GetFieldValueWithIdentifier(s->fieldInstances, "");
		if (axePosJson.hasValue)
		{
			const auto axePos = axePosJson.value.get<LDTKPoint>();

			const auto bridgeRect = Rect::FromXYWH(
				s->px[0], s->px[1], s->width, s->height
			);
			sceneEntities.bridge.Set(BridgeData{
				bridgeRect,
				Vector2Int(axePos.cx * 16, axePos.cy * 16),
			});
		}
	}
}

void LevelLoader::ParseQuestionBlock(SceneEntityData& sceneEntities, vector<EntityInstance> entities)
{
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
}

void LevelLoader::ParseBrickBlock(SceneEntityData& sceneEntities, vector<EntityInstance> entities)
{
	const auto eBlocks = GetEntityDataWithIdentifier(entities, BRICK_BLOCK);
	for (const auto&g : eBlocks)
	{
		auto blockDropJson = GetFieldValueWithIdentifier(g->fieldInstances, "BrickBlockDropType");
		auto isHiddenJson = GetFieldValueWithIdentifier(g->fieldInstances, "is_hidden");

		if (!blockDropJson.hasValue || !isHiddenJson.hasValue)
			continue;

		auto blockDrop = blockDropJson.value.get<string>();
		auto isHidden = isHiddenJson.value.get<bool>();

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


		auto data = BrickBlockData{
			Vector2Int(g->px[0], g->px[1]),
			blockDropValue,
			isHidden,
		};

		sceneEntities.brickBlocks.push_back(data);
	}
}

void LevelLoader::ParseCoin(SceneEntityData& sceneEntities, vector<EntityInstance> entities)
{
	const auto coins = GetEntityDataWithIdentifier(entities, COIN);
	for (const auto& g : coins)
	{
		sceneEntities.coins.emplace_back(g->px[0], g->px[1]);
	}
}

void LevelLoader::ParseNextLevelZone(SceneEntityData& sceneEntities, vector<EntityInstance> entities)
{
	const auto nextLevels = GetEntityDataWithIdentifier(entities, NEXT_LEVEL_ZONE);
	for (const auto& g : nextLevels)
	{
		auto zone = Rect::FromXYWH(g->px[0], g->px[1], g->width, g->height);
		auto nextLevel = GetFieldValueWithIdentifier(g->fieldInstances, "level_to_load");
		auto delay = GetFieldValueWithIdentifier(g->fieldInstances, "load_delay");

		if (!nextLevel.hasValue || !delay.hasValue)
		{
			continue;
		}

		int nextLevelValue = -nextLevel.value.get<int>();
		float delayValue = delay.value.get<float>();

		NextLevelData data{
			zone,
			nextLevelValue,
			delayValue,
		};

		sceneEntities.nextLevelsData.push_back(data);
	}
}

void LevelLoader::ParseBackgroundMusic(int level, SceneEntityData& sceneEntities, vector<EntityInstance> entities)
{
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
}

void LevelLoader::ParseFireballTrap(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities)
{
	const auto traps = GetEntityDataWithIdentifier(entities, FIREBALL_TRAP);
	for (const auto& g : traps)
	{
		sceneEntities.fireballTraps.emplace_back(g->px[0], g->px[1]);
	}
}

void LevelLoader::ParseFlagPole(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities)
{
	const auto flagPoles = GetEntityDataWithIdentifier(entities, FLAG_POLE);
	Optional<FlagPoleData> flagPoleData;
	if (!flagPoles.empty())
	{
		auto flagPole = flagPoles[0]; // only one flag pole per level

		auto moveTo = GetFieldValueWithIdentifier(flagPole->fieldInstances, "player_move_to");
		auto ldtkFireworkPosition = GetFieldValueWithIdentifier(flagPole->fieldInstances, "fireworks_positions");

		if (moveTo.hasValue)
		{
			const auto moveToValue = moveTo.value.get<LDTKPoint>();
			vector<Vector2Int> fireworkPositions;
			
			if (ldtkFireworkPosition.hasValue)
			{
				for (const auto& fireWorkPos : ldtkFireworkPosition.value.get<vector<LDTKPoint>>())
				{
					fireworkPositions.emplace_back(
						fireWorkPos.cx * 16,
						fireWorkPos.cy * 16);
				}
			}
	

			flagPoleData.Set(FlagPoleData{
				Rect::FromXYWH(flagPole->px[0], flagPole->px[1], flagPole->width, flagPole->height),
				Vector2Int(moveToValue.cx * 16, moveToValue.cy * 16),
				fireworkPositions
			});
		}
	}
	sceneEntities.flagPole = flagPoleData;
}

SceneEntityData LevelLoader::ParseEntityLayer(const int level, const vector<LayerInstance>& v)
{
	SceneEntityData sceneEntities;
	
	auto layer = GetLayerWithIdentifier(v, DYNAMIC_LAYER);
	auto entities = layer->entityInstances;

	if (entities.empty())
		return sceneEntities;

	
	ParsePlayerStart(sceneEntities, entities);

	// NOTE: emplace_back is push_back but takes in a constructor, so no temp object creation is needed
	ParseGoombas(sceneEntities, entities);
	ParseKoopas(sceneEntities, entities);
	ParseBowsers(sceneEntities, entities);
	ParseToad(sceneEntities, entities);
	ParseBridge(sceneEntities, entities);
	ParseQuestionBlock(sceneEntities, entities);
	ParseBrickBlock(sceneEntities, entities);
	ParseCoin(sceneEntities, entities);
	ParseNextLevelZone(sceneEntities, entities);
	ParseBackgroundMusic(level, sceneEntities, entities);
	ParseFireballTrap(sceneEntities, entities);
	ParseFlagPole(sceneEntities, entities);

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

Optional<json> LevelLoader::GetFieldValueWithIdentifier(const vector<FieldInstance>& v, const std::string& iden)
{
	Optional<json> returnVal;
	for (auto &f : v)
	{
		if (f.identifier == iden)
		{
			returnVal.Set(f.value);
			break;
		} 
	}
	return returnVal;
}

