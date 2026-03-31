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
void LevelLoader::Init()
{
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
	
	// parse collision layer
	auto layer = GetLayerWithIdentifier(layersValue, COLLISION_LAYER);
	if (layer == nullptr)
		return nullptr;


	int outID;
	if (!layer->tilesetRelPath.hasValue)
	{
		return nullptr;
	}
	
	auto tilesetPath = layer->tilesetRelPath.value;
	auto tileSetWStr = wstring(tilesetPath.begin(), tilesetPath.end());
	if (!t->HaveTextureWithPath(tileSetWStr.c_str(), outID))
	{
		outID = -1 - level;
		t->Add(outID, tileSetWStr.c_str());
	}
	vector<Tile*>  tiles;
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

	// parse background layer
	layer = GetLayerWithIdentifier(layersValue, BACKGROUND_LAYER);
	for (const auto& v : layer->gridTiles)
	{
		auto x = static_cast<int>(v.px[0]);
		auto y = static_cast<int>(v.px[1]);
		auto px = static_cast<int>(v.src[0]);
		auto py = static_cast<int>(v.src[1]);

		Tile tile;
		tile.value = -1;
		tile.x = x;
		tile.y = y;
		tile.px = px;
		tile.py = py;
		tile.width = static_cast<int>(layer->gridSize);
		tile.height = static_cast<int>(layer->gridSize);
		tiles.push_back(&tile);
	}
	

	// parse dynamic (entity) layer
	layer = GetLayerWithIdentifier(layersValue, DYNAMIC_LAYER);
	auto playerStarts = layer->entityInstances[0];
	int x = static_cast<int>(playerStarts.px[0]);
	int y = static_cast<int>(playerStarts.px[1]);
	auto tilemap = new Tilemap(outID, tiles, x, y);

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