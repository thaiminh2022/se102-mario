#pragma once

#include <string>
#include <nloahmann/json.hpp>
#include <Windows.h>
#include <vector>

#include "LdtkParser.h"
#include "Tilemap.h"
#include "Vector2.h"

using std::vector;
using std::wstring;
using std::ifstream;


class LevelLoader
{
	static LevelLoader* _instance;
	vector<Tilemap*> tilemaps;

	// Helper
	Tilemap* ParseLevel(int level);
	static const LayerInstance* GetLayerWithIdentifier(const vector<LayerInstance>& v, const std::string& identifier);
	static RenderLayer ParseCollisionLayer(const vector<LayerInstance>& v, CollisionLayer& col);
	static RenderLayer ParseBackgroundLayer(const vector<LayerInstance>& v);
	static Vector2Int ParseEntityLayer(const vector<LayerInstance>& v);

public:
	static LevelLoader* GetInstance()
	{
		if (_instance == nullptr)
			_instance = new LevelLoader;

		return _instance;
	}

	Tilemap* GetTilemapForLevel(const int level) const { return tilemaps[level]; }
	void Init();
};
