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
using std::string;


class LevelLoader
{
	static LevelLoader* _instance;
	unordered_map<int, Tilemap*> tilemaps;
	unordered_map<string, vector<EntityInstance*>> levelEntitiesCache;
	Optional<WorldMap> worldMap;

	Tilemap* ParseLevel(int level);
	static LayerInstance* GetLayerWithIdentifier(vector<LayerInstance>& v, const string& identifier);
	static CollisionLayer ParseCollisionLayer(vector<LayerInstance>& v);
	static RenderLayer ParseBackgroundLayer(vector<LayerInstance>& v);
	static Optional<RenderLayer> ParseAltLayer(vector<LayerInstance>& v);

	SceneEntityData ParseEntityLayer(int level, vector<LayerInstance>& v);

	// parsing function
	void ParsePlayerStart(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities);
	void ParseGoombas(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities);
	void ParseKoopas(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities);
	void ParseCheepCheeps(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities);
	void ParseWingedKoopas(SceneEntityData& sceneEntities, std::vector<EntityInstance> entities);

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
	void ParseTeleportPipe(SceneEntityData& sceneEntities, vector<EntityInstance>& entities);
	void ParseInstantTeleportPipe(SceneEntityData& sceneEntities, vector<EntityInstance>& entities);
	void ParseClearScreenColorTrigger(SceneEntityData& sceneEntities, vector<EntityInstance>& entities);


	// helper
	void RebuildCacheForLevel(vector<EntityInstance>& entities);
	vector<EntityInstance*> GetEntityDataWithIdentifier(vector<EntityInstance>& v, const std::string& iden);
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
