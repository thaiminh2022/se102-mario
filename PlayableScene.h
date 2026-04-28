#pragma once
#include <queue>

#include "Scene.h"
#include <vector>

#include "GameObject.h"
#include "Timer.h"

using std::vector;

class PlayableScene :
	public Scene
{
    std::queue<GameObject*> addPendingGos;

	vector<GameObject*> objects;
	SceneContext* ctx;
	int level;

	Timer levelTimer;
	float timeLeftForLevel = 160; //seconds, originaly 400 frames w/ each frame ~0.4s
	
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
	float GetTimeLeft() const {
		return levelTimer.GetTimeLeft();
	}
};


