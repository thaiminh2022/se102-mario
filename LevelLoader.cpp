#include "LevelLoader.h"
#include "Debug.h"
#include "Textures.h"

#include <fstream>
#include <vector>
#include <string>

#include "LdtkParser.h"	
#include "Game.h"
#include "Tilemap.h"

using json = nlohmann::json;
using namespace std;
LevelLoader *LevelLoader::_instance = nullptr;



constexpr auto LEVEL_0_TILESET = L"Assets/Sprites/ground_and_stone_overworld.png";
const string WORLD_PATH = "world_map.ldtk";
const string COLLISION_LAYER = "Collision";
const string BACKGROUND_LAYER = "Background";
const string DYNAMIC_LAYER = "Dynamic";


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

void LevelLoader::Init()
{
	const auto t = Textures::GetInstance();
	t->Add(-1, LEVEL_0_TILESET);
}



Tilemap *LevelLoader::ParseLevel(int level)
{
	/// PARSE JSON
	ifstream f(WORLD_PATH);
	json data  = json::parse(f);
	const auto& map = data.get<WorldMap>();

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
	CollisionLayer collisionLayer;
	auto r1 = ParseCollisionLayer(layersValue, collisionLayer);
	auto r2 = ParseBackgroundLayer(layersValue);
	auto playerStart = ParseEntityLayer(layersValue);

	renderLayers.push_back(r1);
	renderLayers.push_back(r2);

	
	const auto config = new TilemapConfig {
		playerStart.x,
		playerStart.y,
		static_cast<int>(levelData.pxWid),
		static_cast<int>(levelData.pxHei),
		16,
		16,
		renderLayers,
		collisionLayer
	};

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

RenderLayer LevelLoader::ParseCollisionLayer(const vector<LayerInstance>& v, CollisionLayer& col)
{
	// collision layer is store as an int grid
	auto layerData = GetLayerWithIdentifier(v, COLLISION_LAYER);
	col.tileWidth = 16;
	col.tileHeight = 16;
	col.cWidth = layerData->cWid;
	col.cHeight = layerData->cHei;

	RenderLayer renderLayer;
	renderLayer.tileWidth = 16;
	renderLayer.tileHeight = 16;
	renderLayer.cellWidth = layerData->cWid;
	renderLayer.cellHeight = layerData->cHei;

	// parse collision
	int i = 0;
	for (const auto value: layerData->intGridCsv)
	{
		CollisionTile t;
		t.tileWidth = 16;
		t.tileHeight = 16;

		t.worldX = i % layerData->cWid * 16;
		t.worldY = i / layerData->cWid * 16;
		
		if (value == 2)
		{
			t.type = CollisionTileType::Ground;
		}else if (value == 3)
		{
			t.type = CollisionTileType::OneWay;
		}else
		{
			t.type = CollisionTileType::None;
		}

		col.cells.push_back(t);
		i++;
	}

	int tId = -1;
	if (!Textures::GetInstance()->HaveTextureWithPath(LEVEL_0_TILESET, tId))
	{
		DebugOut(L"[ERROR] Cannot fine tileset, resolve to default: -1");
	}
	renderLayer.textureID = tId;

	// parse visual
	for (const auto& l: layerData->autoLayerTiles)
	{
		RenderTile t;
		t.worldX = l.px[0];
		t.worldY = l.px[1];
		t.srcX = l.src[0];
		t.srcY = l.src[1];

		t.width = 16;
		t.height = 16;
		renderLayer.tiles.push_back(t);
	}

	return renderLayer;
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

Vector2Int LevelLoader::ParseEntityLayer(const vector<LayerInstance>& v)
{
	auto layer = GetLayerWithIdentifier(v, DYNAMIC_LAYER);
	auto entities = layer->entityInstances;

	if (!entities.empty())
	{
		auto playerStartEntity = layer->entityInstances[0]; // index 0 is player start
		return Vector2Int(playerStartEntity.px[0], playerStartEntity.px[1]);
	}

	return Vector2Int();
}

