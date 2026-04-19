#pragma once
#include <queue>

#include "Scene.h"
#include <vector>

#include "GameObject.h"

using std::vector;

class PlayableScene :
	public Scene
{
    std::queue<GameObject*> addPendingGos;

	vector<GameObject*> objects;
	SceneContext* ctx;
	int level;
	
public:
	int maxFireballs = 2; // Max fireballs allowed on screen at once, to prevent spamming
	explicit PlayableScene(const int level)
	{
		this->level = level;
		ctx = nullptr; // init on load
	}

    void Update(float dt) override;
    void Load() override;
    void UnLoad() override;
    void Render() override;
    void CleanupDeletedObjects();
    void AddObject(GameObject* go);

	int GetMaxFireballs() const {
		return maxFireballs;
	}
};


