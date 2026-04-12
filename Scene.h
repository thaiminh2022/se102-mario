#pragma once
#include <vector>
#include "Tilemap.h"
#include <functional>

class Mario;
class GameObject;
using std::vector;

struct SceneContext
{
	Mario* mario;
	Tilemap* tilemap;
	std::function<void(GameObject*)> addObject;
};

//Abstract class showing the interface of a scene
class Scene
{
protected:
	int id;

public:
	int GetID() const { return id; }

	Scene() { id = -1; }
	explicit Scene(int id) { this->id = id; }
	virtual void Update(float dt) {}
	virtual void Render() {}
	virtual void Load() {}
	virtual void UnLoad() {}

	virtual  ~Scene() = default;
};
