#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <nloahmann/json.hpp>

#include "OptionalType.h"

using json = nlohmann::json;


struct TileInstance
{
	double a; //alpha (opacity)
	int f; // (flip; 0 = no flip; 1 = horizontal flip, 2 = vertical flip 3 = both way)
	std::vector<int> px;
	std::vector<int> src;
	int t;
};

struct FieldInstance
{
	std::string identifier;
	std::string type;
	json value;
	int defUid;
};

struct EntityInstance
{
	std::vector<int> grid;
	std::string identifier;
	std::vector<double> pivot;
	std::string smartColor;
	std::vector<std::string> tags;
	json tile;
	Optional<int> worldX;
	Optional<int> worldY;
	int defUid;
	std::vector<FieldInstance> fieldInstances;
	int height;
	std::string iid;
	std::vector<int> px;
	int width;
};

struct LayerInstance
{
	int cHei;
	int cWid;
	int gridSize;
	std::string identifier;
	double opacity;
	int pxTotalOffsetX;
	int pxTotalOffsetY;
	Optional<int> tilesetDefUid;
	Optional<std::string> tilesetRelPath;
	std::string type;
	std::vector<TileInstance> autoLayerTiles;
	std::vector<EntityInstance> entityInstances;
	std::vector<TileInstance> gridTiles;
	std::string iid;
	std::vector<int> intGridCsv;
	int layerDefUid;
	int levelId;
	Optional<int> overrideTilesetUid;
	int pxOffsetX;
	int pxOffsetY;
	bool visible;
};

struct NeighbourLevel
{
	std::string dir;
	std::string levelIid;
};

struct Level
{
	std::string bgColor;
	json bgPos;
	std::vector<NeighbourLevel> neighbours;
	Optional<std::string> bgRelPath;
	std::vector<FieldInstance> fieldInstances;
	std::string identifier;
	std::string iid;
	Optional<std::vector<LayerInstance> > layerInstances;
	int pxHei;
	int pxWid;
	int uid;
	int worldDepth;
	int worldX;
	int worldY;
};

struct LDTKPoint
{
	int cx;
	int cy;
};

struct LDTKEntityRef
{
	std::string entityIid;
	std::string layerIid;
	std::string levelIid;
	std::string worldIid;
};




struct WorldMap
{
	std::vector<Level> levels;
	std::vector<json> worlds;
	std::string dummyWorldIid;
};

inline const json& RequireField(const json& j, const char* key)
{
	json::const_iterator it = j.find(key);
	if (it == j.end())
		throw std::runtime_error(std::string("Missing required key: ") + key);
	return *it;
}

template <typename T>
inline void ReadRequired(const json& j, const char* key, T& out)
{
	const json& v = RequireField(j, key);
	if (v.is_null())
		throw std::runtime_error(std::string("Required key is null: ") + key);
	out = v.get<T>();
}

template <typename T>
inline void ReadOptional(const json& j, const char* key, Optional<T>& out)
{
	json::const_iterator it = j.find(key);
	if (it == j.end() || it->is_null())
	{
		out.reset();
		return;
	}

	out.emplace(it->get<T>());
}

template <typename T>
inline void ReadOrDefault(const json& j, const char* key, T& out, const T& defaultValue)
{
	json::const_iterator it = j.find(key);
	if (it == j.end() || it->is_null())
	{
		out = defaultValue;
		return;
	}

	out = it->get<T>();
}
inline void from_json(const json& j, LDTKPoint& p)
{
	ReadRequired(j, "cx", p.cx);
	ReadRequired(j, "cy", p.cy);

}

inline void from_json(const json& j, LDTKEntityRef& p)
{
	ReadRequired(j, "entityIid", p.entityIid);
	ReadRequired(j, "layerIid", p.layerIid);
	ReadRequired(j, "levelIid", p.levelIid);
	ReadRequired(j, "worldIid", p.worldIid);
}
inline void from_json(const json& j, TileInstance& x)
{
	ReadRequired(j, "a", x.a);
	ReadRequired(j, "f", x.f);
	ReadRequired(j, "px", x.px);
	ReadRequired(j, "src", x.src);
	ReadRequired(j, "t", x.t);
}

inline void from_json(const json& j, FieldInstance& x)
{
	ReadRequired(j, "__identifier", x.identifier);
	ReadRequired(j, "__type", x.type);
	x.value = RequireField(j, "__value");
	ReadRequired(j, "defUid", x.defUid);
}

inline void from_json(const json& j, EntityInstance& x)
{
	ReadRequired(j, "__grid", x.grid);
	ReadRequired(j, "__identifier", x.identifier);
	ReadRequired(j, "__pivot", x.pivot);
	ReadRequired(j, "__smartColor", x.smartColor);
	ReadRequired(j, "__tags", x.tags);

	x.tile = RequireField(j, "__tile");

	ReadOptional(j, "__worldX", x.worldX);
	ReadOptional(j, "__worldY", x.worldY);

	ReadRequired(j, "defUid", x.defUid);
	ReadRequired(j, "fieldInstances", x.fieldInstances);
	ReadRequired(j, "height", x.height);
	ReadRequired(j, "iid", x.iid);
	ReadRequired(j, "px", x.px);
	ReadRequired(j, "width", x.width);
}

inline void from_json(const json& j, LayerInstance& x)
{
	ReadRequired(j, "__cHei", x.cHei);
	ReadRequired(j, "__cWid", x.cWid);
	ReadRequired(j, "__gridSize", x.gridSize);
	ReadRequired(j, "__identifier", x.identifier);
	ReadRequired(j, "__opacity", x.opacity);
	ReadRequired(j, "__pxTotalOffsetX", x.pxTotalOffsetX);
	ReadRequired(j, "__pxTotalOffsetY", x.pxTotalOffsetY);

	ReadOptional(j, "__tilesetDefUid", x.tilesetDefUid);
	ReadOptional(j, "__tilesetRelPath", x.tilesetRelPath);

	ReadRequired(j, "__type", x.type);
	ReadRequired(j, "autoLayerTiles", x.autoLayerTiles);
	ReadRequired(j, "entityInstances", x.entityInstances);
	ReadRequired(j, "gridTiles", x.gridTiles);
	ReadRequired(j, "iid", x.iid);
	ReadRequired(j, "intGridCsv", x.intGridCsv);
	ReadRequired(j, "layerDefUid", x.layerDefUid);
	ReadRequired(j, "levelId", x.levelId);

	ReadOptional(j, "overrideTilesetUid", x.overrideTilesetUid);

	ReadRequired(j, "pxOffsetX", x.pxOffsetX);
	ReadRequired(j, "pxOffsetY", x.pxOffsetY);
	ReadRequired(j, "visible", x.visible);
}

inline void from_json(const json& j, NeighbourLevel& x)
{
	ReadRequired(j, "dir", x.dir);
	ReadRequired(j, "levelIid", x.levelIid);
}

inline void from_json(const json& j, Level& x)
{
	ReadRequired(j, "__bgColor", x.bgColor);

	x.bgPos = RequireField(j, "__bgPos");

	ReadRequired(j, "__neighbours", x.neighbours);
	ReadOptional(j, "bgRelPath", x.bgRelPath);
	ReadRequired(j, "fieldInstances", x.fieldInstances);
	ReadRequired(j, "identifier", x.identifier);
	ReadRequired(j, "iid", x.iid);
	ReadOptional(j, "layerInstances", x.layerInstances);
	ReadRequired(j, "pxHei", x.pxHei);
	ReadRequired(j, "pxWid", x.pxWid);
	ReadRequired(j, "uid", x.uid);
	ReadRequired(j, "worldDepth", x.worldDepth);
	ReadRequired(j, "worldX", x.worldX);
	ReadRequired(j, "worldY", x.worldY);
}

inline void from_json(const json& j, WorldMap& x)
{
	ReadRequired(j, "levels", x.levels);
	ReadRequired(j, "worlds", x.worlds);
	ReadRequired(j, "dummyWorldIid", x.dummyWorldIid);
}

