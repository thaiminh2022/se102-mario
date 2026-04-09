#pragma once

#include <string>
#include <vector>

#include "LdtkParser.h"
#include "SceneEntityData.h"
#include "Tile.h"
#include "Tilemap.h"
#include "unordered_map"
#include <fstream>

using std::vector;
using std::wstring;
using std::ifstream;
using std::unordered_map;


class LevelLoader
{
	static LevelLoader* _instance;
	unordered_map<int, Tilemap*> tilemaps;
	Optional<WorldMap> worldMap;

	// Helpers
	Tilemap* ParseLevel(int level);
	static const LayerInstance* GetLayerWithIdentifier(const vector<LayerInstance>& v, const std::string& identifier);
	static CollisionLayer ParseCollisionLayer(const vector<LayerInstance>& v);
	static RenderLayer ParseBackgroundLayer(const vector<LayerInstance>& v);
	static SceneEntityData ParseEntityLayer(int level, const vector<LayerInstance>& v);
	static vector<EntityInstance*> GetEntityDataWithIdentifier(vector<EntityInstance>& v, const std::string& iden);

public:
	static LevelLoader* GetInstance()
	{
		if (_instance == nullptr)
			_instance = new LevelLoader;

		return _instance;
	}

	Tilemap* GetTilemapForLevel(const int level);
	void Init();
};
