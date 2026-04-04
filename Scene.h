#pragma once
#include <Windows.h>
#include <vector>

#include "Tilemap.h"

using std::vector;
struct SceneContext
{
	Tilemap* tilemap;
};


class Scene
{
protected:
	int id;

public:
	int GetID() { return id; }

	Scene() { id = -1; }
	Scene(int id) { this->id = id; }
	virtual void Update(DWORD dt) {}
	virtual void Render() {}
	virtual void Load() {}
	virtual void UnLoad() {}

	virtual  ~Scene(){}
};
