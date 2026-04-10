#pragma once
#include "Scene.h"
#include <vector>

#include "GameObject.h"
#include "Mario.h"

using std::vector;

class PlayableScene :
	public Scene
{

	Mario* player;
	vector<GameObject*> objects;
	vector<GameObject*> newObjects;
	SceneContext* ctx;
	
public:
	int maxFireballs = 2; // Max fireballs allowed on screen at once, to prevent spamming
	explicit PlayableScene(const int level) : Scene(level)
	{
		player = nullptr; // init on load
		ctx = nullptr; // init on load
	}

	void Update(float dt) override;
	void Load() override;
	void UnLoad() override;
	void Render() override;
	void CleanupDeletedObjects();
	void AddObject(GameObject* obj) {
		newObjects.push_back(obj);
	}
	int getMaxFireballs() const {
		return maxFireballs;
	}
	int GetActiveFireballsCount();
};


