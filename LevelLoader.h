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

	Tilemap* ParseLevel(int level);
	static const LayerInstance* GetLayerWithIdentifier(const vector<LayerInstance>& v, const std::string& identifier);
	static CollisionLayer ParseCollisionLayer(const vector<LayerInstance>& v);
	static RenderLayer ParseBackgroundLayer(const vector<LayerInstance>& v);
	SceneEntityData ParseEntityLayer(int level, const vector<LayerInstance>& v);

	// parsing function
	void ParsePlayerStart(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities);
	void ParseGoombas(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities);
	void ParseKoopas(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities);
	void ParseBowsers(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities);
	void ParseToad(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities);
	void ParseBridge(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities);
	void ParseQuestionBlock(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities);
	void ParseBrickBlock(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities);
	void ParseCoin(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities);
	void ParseNextLevelZone(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities);
	void ParseBackgroundMusic(int level, SceneEntityData& sceneEntities, std::vector<EntityInstance> entities);
	void ParseFireballTrap(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities);
	void ParseFlagPole(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities);
	void ParsePipe(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities);
	
	// helper
	static vector<EntityInstance*> GetEntityDataWithIdentifier(vector<EntityInstance>& v, const std::string& iden);
	static Optional<json> GetFieldValueWithIdentifier(const vector<FieldInstance>& v, const std::string& iden);
	Optional<EntityInstance> ParseEntityRef(const LDTKEntityRef& entityRef) const;


public:
	static LevelLoader* GetInstance()
	{
		if (_instance == nullptr)
			_instance = new LevelLoader;

		return _instance;
	}

	Tilemap* GetTilemapForLevel(int level);
	void Init();
};
