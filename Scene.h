#pragma once
#include <Windows.h>
#include <vector>

#include "Tilemap.h"

using std::vector;
struct SceneContext
{
	Tilemap* tilemap;
};

//Abstract class showing the interface of a scene
class Scene
{
protected:
	int id;

public:
	int GetID() { return id; }

	Scene() { id = -1; }
	Scene(int id) { this->id = id; }
	virtual void Update(float dt) {}
	virtual void Render() {}
	virtual void Load() {}
	virtual void UnLoad() {}

	virtual  ~Scene() = default;
};
