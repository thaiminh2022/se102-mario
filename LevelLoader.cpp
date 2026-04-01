#include "LevelLoader.h"

#include "Debug.h"

#include "Textures.h"
#include <filesystem>

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

void LevelLoader::Init()
{
	const auto t = Textures::GetInstance();
	t->Add(-1, LEVEL_0_TILESET);

	tilemaps.push_back(ParseLevel(0));
}

const string WORLD_PATH = "world_map.ldtk";
const string COLLISION_LAYER = "Collision";
const string BACKGROUND_LAYER = "Background";
const string DYNAMIC_LAYER = "Dynamic";


Tilemap *LevelLoader::ParseLevel(int level)
{
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
	auto t = Textures::GetInstance();
	

	// parse background layer
	auto layer = GetLayerWithIdentifier(layersValue, BACKGROUND_LAYER);
	vector<Tile*> tiles;

	for (const auto& v : layer->gridTiles)
	{
		auto x = static_cast<int>(v.px[0]);
		auto y = static_cast<int>(v.px[1]);
		auto px = static_cast<int>(v.src[0]);
		auto py = static_cast<int>(v.src[1]);

		auto tile = new Tile;
		tile->value = -1;
		tile->x = x;
		tile->y = y;
		tile->px = px;
		tile->py = py;
		tile->width = static_cast<int>(layer->gridSize);
		tile->height = static_cast<int>(layer->gridSize);
		tiles.push_back(tile);
	}

	// parse collision layer
	layer = GetLayerWithIdentifier(layersValue, COLLISION_LAYER);
	if (layer == nullptr)
		return nullptr;

	for (const auto &v: layer->autoLayerTiles)
	{
		auto x = static_cast<int>(v.px[0]);
		auto y = static_cast<int>(v.px[1]);
		auto px = static_cast<int>(v.src[0]);
		auto py = static_cast<int>(v.src[1]);

		auto tile = new Tile;
		int64_t valueIndex = px + py / layer->gridSize;
		tile->value = static_cast<int>(layer->intGridCsv[valueIndex]);
		tile->x = x;
		tile->y = y;
		tile->px = px;
		tile->py = py;
		tile->width = static_cast<int>(layer->gridSize);
		tile->height = static_cast<int>(layer->gridSize);
		tiles.push_back(tile);
	}


	

	// parse dynamic (entity) layer
	layer = GetLayerWithIdentifier(layersValue, DYNAMIC_LAYER);
	auto playerStarts = layer->entityInstances[0];
	int x = static_cast<int>(playerStarts.px[0]);
	int y = static_cast<int>(playerStarts.px[1]);

	
	int outID;
	t->HaveTextureWithPath(LEVEL_0_TILESET, outID);
	const auto tilemap = new Tilemap(outID, tiles, x, y);

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