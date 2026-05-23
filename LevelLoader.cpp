#include "Debug.h"
#include "LevelLoader.h"
#include "Textures.h"

#include <fstream>
#include <string>
#include <vector>

#include "AssetIDs.h"
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
LevelLoader* LevelLoader::_instance = nullptr;




const string WORLD_PATH = "world_map-new.ldtk";

// LAYER
const string COLLISION_LAYER = "Collision";
const string BACKGROUND_LAYER = "Background";
const string ALT_LAYER = "AltBackground";
const string DYNAMIC_LAYER = "Dynamic";

// ENTITY
const string PLAYER_START = "PlayerStart";
const string GOOMBA_START = "GoombaStart";
const string KOOPA_START = "KoopaStart";
const string CHEEP_CHEEPS_START = "FishStart";
const string BLOOPER_START = "SquidStart";

const string WINGED_KOOPA_START = "WingedKoopaStart";
const string QUESTION_BLOCK = "QuestionBlock";
const string BRICK_BLOCK = "EmptyBrickBlock";
const string COIN = "Coin";
const string FIREBALL_TRAP = "FireballTrap";
const string FLAG_POLE = "FlagPole";
const string BOWSER_START = "BowserStart";
const string BRIDGE = "Bridge";
const string PIPE = "Pipe";
const string TELEPORT_PIPE = "TeleportPipe";
const string INSTANT_TELEPORT_PIPE = "InstantTeleportPipe";
const string FIRE_SHOOTER = "FireShooter";
const string BOWSER_ARENA = "BowserArena";

// TRIGGERS
const string CLRSCR_COLOR_TRIGGER = "ClearScreenColorTrigger";
const string IN_WATER_TRIGGER = "WaterTrigger";
const string NEXT_LEVEL_ZONE = "NextLevel";
const string BACKGROUND_MUSIC = "BackgroundMusic";
const string ENTER_CASTLE_TRIGGER = "EnterCastleTrigger";


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
	// world texture loading

	const auto t = Textures::GetInstance();
	t->Add(-1, L"Assets/Sprites/tileset_overworld.png");
	t->Add(-2, L"Assets/Sprites/tileset_underground.png");
	t->Add(-3, L"Assets/Sprites/tileset_castle.png");
	t->Add(-4, L"Assets/Sprites/tileset_water.png");


	// world music loading
	const auto audio = AudioManager::GetInstance();
	audio->LoadWAV(GROUND_THEME, L"Assets/Audio/Soundtracks/01.GroundTheme.wav");
	audio->LoadWAV(UNDERGROUND_THEME, L"Assets/Audio/Soundtracks/02.UndergroundTheme.wav");
	audio->LoadWAV(UNDERWATER_THEME, L"Assets/Audio/Soundtracks/03.UnderwaterTheme.wav");
	audio->LoadWAV(CASTLE_THEME, L"Assets/Audio/Soundtracks/04.CastleTheme.wav");


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
	{
		// what happened lol, though you got init innit?
		Init();
	}

	auto& map = worldMap.value;
	if (level >= map.levels.size())
		return nullptr;

	auto& levelData = map.levels[level];
	if (!levelData.layerInstances.hasValue)
	{
		return nullptr;
	}
	auto& layersValue = levelData.layerInstances.value;
	/// -----
	vector<RenderLayer> renderLayers;
	const auto col = ParseCollisionLayer(layersValue);
	const auto r1 = ParseBackgroundLayer(layersValue);
	const auto r2 = ParseAltLayer(layersValue);
	const auto entitiesData = ParseEntityLayer(level, layersValue);
	renderLayers.push_back(r1);

	if (r2.hasValue)
	{
		renderLayers.push_back(r2.value);
	}


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
		}
		else if (biome == "Underground")
		{
			levelBiome = BiomeType::Underground;
		}
		else if (biome == "Castle")
		{
			levelBiome = BiomeType::Castle;
		}
		else if (biome == "Water")
		{
			levelBiome = BiomeType::UnderWater;
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

LayerInstance* LevelLoader::GetLayerWithIdentifier(
	vector<LayerInstance>& v,
	const string& identifier)
{
	for (auto& layer : v)
	{
		if (layer.identifier == identifier)
		{
			return &layer;
		}
	}
	return nullptr;
}

CollisionLayer LevelLoader::ParseCollisionLayer(vector<LayerInstance>& v)
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
	for (const auto value : layerData->intGridCsv)
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



RenderLayer LevelLoader::ParseBackgroundLayer(vector<LayerInstance>& v)
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
		DebugOut(L"[ERROR] Cannot find tileset");
		throw;
	}
	renderLayer.textureID = tID;
	renderLayer.tileWidth = layerData->gridSize;
	renderLayer.tileHeight = layerData->gridSize;
	renderLayer.cellWidth = layerData->cWid;
	renderLayer.cellHeight = layerData->cHei;
	renderLayer.tileLookup.assign(renderLayer.cellWidth * renderLayer.cellHeight, -1);

	for (auto l : layerData->gridTiles)
	{
		RenderTile t;
		t.worldX = l.px[0];
		t.worldY = l.px[1];
		t.srcX = l.src[0];
		t.srcY = l.src[1];

		t.width = renderLayer.tileWidth;
		t.height = renderLayer.tileHeight;

		const int tileIndex = static_cast<int>(renderLayer.tiles.size());
		renderLayer.tiles.push_back(t);
		const int cx = t.worldX / renderLayer.tileWidth;
		const int cy = t.worldY / renderLayer.tileHeight;
		renderLayer.tileLookup[cy * renderLayer.cellWidth + cx] = tileIndex;
	}
	return renderLayer;
}

Optional<RenderLayer> LevelLoader::ParseAltLayer(vector<LayerInstance>& v)
{
	auto layerData = GetLayerWithIdentifier(v, ALT_LAYER);
	auto renderLayer = RenderLayer();
	int tID = -1;

	auto texturePath = layerData->tilesetRelPath;
	wstring path;

	if (texturePath.hasValue)
	{
		path = wstring(texturePath.value.begin(), texturePath.value.end());
	}
	else
	{
		return {};
	}

	if (!Textures::GetInstance()->HaveTextureWithPath(path, tID))
	{
		DebugOut(L"[ERROR] Cannot fine tileset");
		throw;
	}
	renderLayer.textureID = tID;
	renderLayer.tileWidth = layerData->gridSize;
	renderLayer.tileHeight = layerData->gridSize;
	renderLayer.cellWidth = layerData->cWid;
	renderLayer.cellHeight = layerData->cHei;
	renderLayer.tileLookup.assign(renderLayer.cellWidth * renderLayer.cellHeight, -1);

	for (auto l : layerData->gridTiles)
	{
		RenderTile t;
		t.worldX = l.px[0];
		t.worldY = l.px[1];
		t.srcX = l.src[0];
		t.srcY = l.src[1];

		t.width = renderLayer.tileWidth;
		t.height = renderLayer.tileHeight;

		const int tileIndex = static_cast<int>(renderLayer.tiles.size());
		renderLayer.tiles.push_back(t);
		const int cx = t.worldX / renderLayer.tileWidth;
		const int cy = t.worldY / renderLayer.tileHeight;
		renderLayer.tileLookup[cy * renderLayer.cellWidth + cx] = tileIndex;
	}
	if (renderLayer.tiles.empty())
	{
		return {};
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
	for (const auto& g : goombas)
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

void LevelLoader::ParseCheepCheeps(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities)
{
	const auto cheepCheeps = GetEntityDataWithIdentifier(entities, CHEEP_CHEEPS_START);
	for (const auto& g : cheepCheeps)
	{
		const auto isRedJson = GetFieldValueWithIdentifier(g->fieldInstances, "is_red");
		if (!isRedJson.hasValue)
			continue;

		sceneEntities.cheepCheeps.emplace_back(
			Vector2Int(g->px[0], g->px[1]),
			isRedJson.value.get<bool>());
	}
}

void LevelLoader::ParseBloopers(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities)
{
	const auto bloopers = GetEntityDataWithIdentifier(entities, BLOOPER_START);
	for (const auto& g : bloopers)
	{
		const auto lowestLimitJson = GetFieldValueWithIdentifier(g->fieldInstances, "lowest_limit");
		const auto highestLimitJson = GetFieldValueWithIdentifier(g->fieldInstances, "highest_limit");


		if (!lowestLimitJson.hasValue || !highestLimitJson.hasValue)
			continue;

		const auto lowestLimit = lowestLimitJson.value.get<LDTKPoint>();
		const auto highestLimit = highestLimitJson.value.get<LDTKPoint>();

		sceneEntities.bloopers.emplace_back(
			Vector2Int(lowestLimit.cx * 16, lowestLimit.cy * 16),
			Vector2Int(highestLimit.cx * 16, highestLimit.cy * 16));
	}
}

void LevelLoader::ParseWingedKoopas(SceneEntityData& sceneEntities, vector<EntityInstance> entities)
{
	const auto wingedKoopas = GetEntityDataWithIdentifier(entities, WINGED_KOOPA_START);
	for (const auto& g : wingedKoopas)
	{
		sceneEntities.WingedKoopaStarts.emplace_back(g->px[0], g->px[1]);
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



void LevelLoader::ParseBridge(SceneEntityData& sceneEntities, vector<EntityInstance> entities)
{
	const auto bridge = GetEntityDataWithIdentifier(entities, BRIDGE);
	if (!bridge.empty())
	{
		const auto s = bridge[0]; // only 1 per level;
		auto axePosJson = GetFieldValueWithIdentifier(s->fieldInstances, "AxePosition");
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

		}
		else if (blockDrop == "JewDestroyer")
		{
			blockDropValue = BlockDropType::JewDestroyer;

		}
		else if (blockDrop == "Starman")
		{
			blockDropValue = BlockDropType::Starman;

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
	for (const auto& g : eBlocks)
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
	const auto bgMusics = GetEntityDataWithIdentifier(entities, BACKGROUND_MUSIC);
	Optional<int> bgMusicId;
	for (const auto& bgMusic : bgMusics)
	{
		const auto audioJson = GetFieldValueWithIdentifier(bgMusic->fieldInstances, "Audio");
		const auto isTriggerJson = GetFieldValueWithIdentifier(bgMusic->fieldInstances, "is_trigger");

		if (!audioJson.hasValue || !isTriggerJson.hasValue)
		{
			continue;
		}

		const auto audioPath = audioJson.value.get<string>();
		const auto isTrigger = isTriggerJson.value.get<bool>();

		const auto utf16String = wstring(audioPath.begin(), audioPath.end());

		auto idData = AudioManager::GetInstance()
			->GetIdForWAVFile(utf16String.c_str());

		if (isTrigger == false)
		{
			bgMusicId = idData.value;
		}

		if (!idData.hasValue)
			continue;
		const auto rect = Rect::FromXYWH(bgMusic->px[0],
			bgMusic->px[1],
			bgMusic->width,
			bgMusic->height
		);

		sceneEntities.musicTriggers.push_back({ idData.value, rect });
	}
	sceneEntities.backgroundMusicID = bgMusicId;
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

		if (moveTo.hasValue)
		{
			const auto moveToValue = moveTo.value.get<LDTKPoint>();



			flagPoleData.Set(FlagPoleData{
				Rect::FromXYWH(flagPole->px[0], flagPole->px[1], flagPole->width, flagPole->height),
				Vector2Int(moveToValue.cx * 16, moveToValue.cy * 16),
				});
		}
	}
	sceneEntities.flagPole = flagPoleData;
}

void LevelLoader::ParsePipe(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities)
{
	const auto pipes = GetEntityDataWithIdentifier(entities, PIPE);
	for (auto p : pipes)
	{
		auto isReturnJson = GetFieldValueWithIdentifier(p->fieldInstances, "is_return_pipe");
		auto pipeDirectionJson = GetFieldValueWithIdentifier(p->fieldInstances, "pipe_direction");
		auto pipeRefJson = GetFieldValueWithIdentifier(p->fieldInstances, "return_pipe_ref");
		auto nextLevelJson = GetFieldValueWithIdentifier(p->fieldInstances, "level_to_load");
		auto moveToJson = GetFieldValueWithIdentifier(p->fieldInstances, "move_to");

		if (!isReturnJson.hasValue || !pipeDirectionJson.hasValue || !moveToJson.hasValue)
			continue;

		auto zone = Rect::FromXYWH(p->px[0], p->px[1], p->width, p->height);
		bool isReturnPipe = isReturnJson.value.get<bool>();
		auto pipeDir = pipeDirectionJson.value.get<string>();
		auto moveTo = moveToJson.value.get<LDTKPoint>();

		// parse next level
		Optional<int> nextLevel;
		if (nextLevelJson.hasValue && !nextLevelJson.value.is_null())
		{
			nextLevel.Set(-nextLevelJson.value.get<int>());
		}

		// parse return pipe
		Optional<ReturnPipeData> returnPipeData;
		if (pipeRefJson.hasValue && !pipeRefJson.value.is_null())
		{
			auto entityRef = pipeRefJson.value.get<LDTKEntityRef>();
			auto otherPipeData = ParseEntityRef(entityRef);

			if (otherPipeData.hasValue)
			{
				const auto& otherPipe = otherPipeData.value;
				auto otherReturnJson = GetFieldValueWithIdentifier(otherPipe.fieldInstances, "is_return_pipe");
				auto otherDirJson = GetFieldValueWithIdentifier(otherPipe.fieldInstances, "pipe_direction");
				auto otherMoveToJson = GetFieldValueWithIdentifier(otherPipe.fieldInstances, "move_to");

				if (otherReturnJson.hasValue && otherDirJson.hasValue && otherMoveToJson.hasValue)
				{
					auto otherMoveTo = otherMoveToJson.value.get<LDTKPoint>();

					returnPipeData.Set({
						PipeData::GetDirection(otherDirJson.value),
						Rect::FromXYWH(otherPipe.px[0], otherPipe.px[1], otherPipe.width, otherPipe.height),
						Vector2Int(otherMoveTo.cx * 16, otherMoveTo.cy * 16)
						});
				}
			}
		}

		sceneEntities.pipes.push_back(PipeData(zone,
			nextLevel,
			returnPipeData,
			isReturnPipe,
			PipeData::GetDirection(pipeDir),
			Vector2Int(moveTo.cx * 16, moveTo.cy * 16),
			{},
			false));
	}
}

void LevelLoader::ParseTeleportPipe(SceneEntityData& sceneEntities, vector<EntityInstance>& entities)
{
	const auto pipes = GetEntityDataWithIdentifier(entities, TELEPORT_PIPE);
	for (auto p : pipes)
	{
		auto isReturnJson = GetFieldValueWithIdentifier(p->fieldInstances, "is_return_pipe");
		auto pipeDirectionJson = GetFieldValueWithIdentifier(p->fieldInstances, "pipe_direction");
		auto pipeRefJson = GetFieldValueWithIdentifier(p->fieldInstances, "teleport_to");
		auto moveToJson = GetFieldValueWithIdentifier(p->fieldInstances, "move_to");

		if (!isReturnJson.hasValue || !pipeDirectionJson.hasValue || !moveToJson.hasValue)
			continue;

		auto zone = Rect::FromXYWH(p->px[0], p->px[1], p->width, p->height);
		bool isReturnPipe = isReturnJson.value.get<bool>();
		auto pipeDir = pipeDirectionJson.value.get<string>();
		auto moveTo = moveToJson.value.get<LDTKPoint>();

		// parse return pipe
		Optional<ReturnPipeData> returnPipeData;
		if (pipeRefJson.hasValue && !pipeRefJson.value.is_null())
		{
			auto entityRef = pipeRefJson.value.get<LDTKEntityRef>();
			auto otherPipeData = ParseEntityRef(entityRef);

			if (otherPipeData.hasValue)
			{
				const auto& otherPipe = otherPipeData.value;
				auto otherReturnJson = GetFieldValueWithIdentifier(otherPipe.fieldInstances, "is_return_pipe");
				auto otherDirJson = GetFieldValueWithIdentifier(otherPipe.fieldInstances, "pipe_direction");
				auto otherMoveToJson = GetFieldValueWithIdentifier(otherPipe.fieldInstances, "move_to");

				if (otherReturnJson.hasValue && otherDirJson.hasValue && otherMoveToJson.hasValue)
				{
					auto otherMoveTo = otherMoveToJson.value.get<LDTKPoint>();

					returnPipeData.Set({
						PipeData::GetDirection(otherDirJson.value),
						Rect::FromXYWH(otherPipe.px[0], otherPipe.px[1], otherPipe.width, otherPipe.height),
						Vector2Int(otherMoveTo.cx * 16, otherMoveTo.cy * 16)
						});
				}
			}
		}
		sceneEntities.pipes.push_back(PipeData(zone,
			{},
			returnPipeData,
			isReturnPipe,
			PipeData::GetDirection(pipeDir),
			Vector2Int(moveTo.cx * 16, moveTo.cy * 16),
			{},
			true));
	}
}

void LevelLoader::ParseInstantTeleportPipe(SceneEntityData& sceneEntities, vector<EntityInstance>& entities)
{
	const auto pipes = GetEntityDataWithIdentifier(entities, INSTANT_TELEPORT_PIPE);
	for (auto p : pipes)
	{
		auto pipeDirectionJson = GetFieldValueWithIdentifier(p->fieldInstances, "pipe_direction");
		auto moveToJson = GetFieldValueWithIdentifier(p->fieldInstances, "move_to");
		auto teleportToJson = GetFieldValueWithIdentifier(p->fieldInstances, "teleport_to");


		if (!pipeDirectionJson.hasValue || !moveToJson.hasValue || !teleportToJson.hasValue)
			continue;

		auto zone = Rect::FromXYWH(p->px[0], p->px[1], p->width, p->height);
		auto pipeDir = pipeDirectionJson.value.get<string>();
		auto moveTo = moveToJson.value.get<LDTKPoint>();
		auto teleportTo = teleportToJson.value.get<LDTKPoint>();

		sceneEntities.pipes.push_back(PipeData(zone,
			{},
			{},
			false,
			PipeData::GetDirection(pipeDir),
			Vector2Int(moveTo.cx * 16, moveTo.cy * 16),
			Vector2Int(teleportTo.cx * 16, teleportTo.cy * 16),
			true));
	}
}

void LevelLoader::ParseClearScreenColorTrigger(SceneEntityData& sceneEntities,
	vector<EntityInstance>& entities)
{
	const auto clrscrTriggers = GetEntityDataWithIdentifier(entities, CLRSCR_COLOR_TRIGGER);

	for (const auto& clrscrTrigger : clrscrTriggers)
	{
		const auto& colorJson = GetFieldValueWithIdentifier(clrscrTrigger->fieldInstances, "color");
		if (!colorJson.hasValue)
			continue;

		const auto colorHex = colorJson.value.get<string>();
		const auto zone = Rect::FromXYWH(clrscrTrigger->px[0], clrscrTrigger->px[1], clrscrTrigger->width, clrscrTrigger->height);
		const auto color = Color(colorHex);

		sceneEntities.clearScreenColorTriggers.emplace_back(zone, color);
	}
}

void LevelLoader::ParseInWaterTrigger(SceneEntityData& sceneEntities, vector<EntityInstance>& entities)
{
	const auto waterTriggers = GetEntityDataWithIdentifier(entities, IN_WATER_TRIGGER);

	for (const auto& waterTrigger : waterTriggers)
	{
		const auto& inWaterJson = GetFieldValueWithIdentifier(waterTrigger->fieldInstances, "in_water");
		if (!inWaterJson.hasValue)
			continue;

		const auto inWater = inWaterJson.value.get<bool>();
		const auto zone = Rect::FromXYWH(waterTrigger->px[0], waterTrigger->px[1], waterTrigger->width, waterTrigger->height);
		sceneEntities.waterTriggers.emplace_back(zone, inWater);
	}
}

void LevelLoader::ParseEnterCastleTrigger(SceneEntityData& sceneEntities, vector<EntityInstance>& entities)
{
	const auto inCastleTriggers = GetEntityDataWithIdentifier(entities, ENTER_CASTLE_TRIGGER);
	if (inCastleTriggers.empty())
		return;

	// can only have 1 per level
	const auto inCastleTrigger = inCastleTriggers[0];

	// firework positions
	const auto fireworkPosData = GetFieldValueWithIdentifier(inCastleTrigger->fieldInstances, "firework_positions");
	const auto flagMoveToData = GetFieldValueWithIdentifier(inCastleTrigger->fieldInstances, "flag_move_to");

	if (!fireworkPosData.hasValue)
		return;

	EnterCastleTriggerData data;
	data.zone = Rect::FromXYWH(inCastleTrigger->px[0], inCastleTrigger->px[1], inCastleTrigger->width, inCastleTrigger->height);

	// parse firework pos to data
	for (const auto& ldtkPos : fireworkPosData.value.get<vector<LDTKPoint>>())
	{
		data.fireworkPositions.emplace_back(ldtkPos.cx * 16, ldtkPos.cy * 16);
	}

	// parse flag move to
	if (!flagMoveToData.hasValue || flagMoveToData.value.is_null())
		return;

	const auto flagMoveTo = flagMoveToData.value.get<LDTKPoint>();
	data.flagMoveTo = Vector2Int(flagMoveTo.cx * 16, flagMoveTo.cy * 16);

	sceneEntities.enterCastleTrigger = data;

}

void LevelLoader::ParseFireShooter(SceneEntityData& sceneEntities, vector<EntityInstance>& entities)
{
	const auto fireShooters = GetEntityDataWithIdentifier(entities, FIRE_SHOOTER);

	for (const auto& fireShooter : fireShooters)
	{
		const auto& fireShooterJson = GetFieldValueWithIdentifier(fireShooter->fieldInstances, "Direction");
		if (!fireShooterJson.hasValue)
			continue;

		const auto directionString = fireShooterJson.value.get<string>();
		Vector2Int direction;
		if (directionString == "Up") {
			direction = Vector2Int::Up();
		}
		else if (directionString == "Down") {
			direction = Vector2Int::Down();
		}
		else if (directionString == "Left") {
			direction = Vector2Int::Left();
		}
		else if (directionString == "Right") {
			direction = Vector2Int::Right();
		}
		else {
			DebugOut(L"[Error] Invalid fire shooter direction");
			direction = Vector2Int::Left();
		}
		const auto position = Vector2Int(fireShooter->px[0], fireShooter->px[1]);
		sceneEntities.fireShooters.emplace_back(position, direction);
	}
}

void LevelLoader::ParseBowserArena(SceneEntityData& sceneEntities, vector<EntityInstance>& entities)
{
	const auto bowserArenas = GetEntityDataWithIdentifier(entities, BOWSER_ARENA);
	if (bowserArenas.empty())
		return;
	auto arena = bowserArenas[0];
	Rect rect = Rect::FromXYWH(
		arena->px[0],
		arena->px[1],
		arena->width,
		arena->height
	);
	sceneEntities.bowserArenas = BowserArenaData(rect);
}

void LevelLoader::RebuildCacheForLevel(vector<EntityInstance>& entities)
{
	levelEntitiesCache.clear();
	for (auto& e : entities)
	{
		levelEntitiesCache[e.identifier].push_back(&e);
	}
}

SceneEntityData LevelLoader::ParseEntityLayer(const int level, vector<LayerInstance>& v)
{
	SceneEntityData sceneEntities{};
	const auto layer = GetLayerWithIdentifier(v, DYNAMIC_LAYER);
	auto& entities = layer->entityInstances;

	if (entities.empty())
		return sceneEntities;

	RebuildCacheForLevel(entities);

	ParsePlayerStart(sceneEntities, entities);

	// NOTE: emplace_back is push_back but takes in a constructor, so no temp object creation is needed

	// Entities
	ParseGoombas(sceneEntities, entities);
	ParseKoopas(sceneEntities, entities);
	ParseCheepCheeps(sceneEntities, entities);
	ParseBloopers(sceneEntities, entities);
	ParseWingedKoopas(sceneEntities, entities);
	ParseBowsers(sceneEntities, entities);
	ParseFireballTrap(sceneEntities, entities);

	// collectables
	ParseQuestionBlock(sceneEntities, entities);
	ParseBrickBlock(sceneEntities, entities);
	ParseCoin(sceneEntities, entities);

	// gameplay
	ParseBridge(sceneEntities, entities);
	ParseFlagPole(sceneEntities, entities);
	ParsePipe(sceneEntities, entities);
	ParseTeleportPipe(sceneEntities, entities);
	ParseInstantTeleportPipe(sceneEntities, entities);
	ParseFireShooter(sceneEntities, entities);
	ParseBowserArena(sceneEntities, entities);

	// triggers
	ParseNextLevelZone(sceneEntities, entities);
	ParseBackgroundMusic(level, sceneEntities, entities);
	ParseClearScreenColorTrigger(sceneEntities, entities);
	ParseInWaterTrigger(sceneEntities, entities);
	ParseEnterCastleTrigger(sceneEntities, entities);

	return sceneEntities;
}

vector<EntityInstance*> LevelLoader::GetEntityDataWithIdentifier(vector<EntityInstance>& v, const string& iden)
{
	if (!levelEntitiesCache.empty())
	{
		return levelEntitiesCache[iden];
	}

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
	for (auto& f : v)
	{
		if (f.identifier == iden)
		{
			returnVal.Set(f.value);
			break;
		}
	}
	return returnVal;
}

Optional<EntityInstance> LevelLoader::ParseEntityRef(const LDTKEntityRef& entityRef) const
{
	if (!worldMap.hasValue)
		return {};
	auto worldMapValue = worldMap.value;

	for (auto& lvl : worldMapValue.levels)
	{
		if (lvl.iid != entityRef.levelIid)
			continue;

		for (auto& layer : lvl.layerInstances.value)
		{
			if (layer.iid != entityRef.layerIid)
				continue;

			for (auto& e : layer.entityInstances)
			{
				if (e.iid == entityRef.entityIid)
				{
					return e;
				}
			}
		}
	}
	return {};
}

