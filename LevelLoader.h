#pragma once

#include <string>
#include <json.hpp>
#include <Windows.h>
#include <vector>

#include "LdtkParser.h"
#include "Tilemap.h"

using std::vector;
using std::wstring;
using std::ifstream;


class LevelLoader
{
	static LevelLoader *_instance;
	vector<Tilemap*> tilemaps;

	// Helper
	static Tilemap *ParseLevel(int level);
	static const LayerInstance* GetLayerWithIdentifier(const vector<LayerInstance>& v, const std::string& identifier);

public:
	static LevelLoader *GetInstance()
	{
		if (_instance == nullptr)
			_instance = new LevelLoader;

		return _instance;
	}

	Tilemap *GetTilemapForLevel(const int level) const { return tilemaps[level]; }
	void Init();
};
