#pragma once
#include <vector>
#include "Tilemap.h"
#include <functional>

#include "SceneSwitchContext.h"

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

public:
	virtual void Update(float dt) {}
	virtual void Render() {}
	virtual void Load(const Optional<SceneSwitchContext>& ctx) {}
	virtual void UnLoad() {}

	virtual  ~Scene() = default;
};
