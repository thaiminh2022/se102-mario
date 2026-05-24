#pragma once
#include <memory>
#include <queue>

#include "Scene.h"
#include <vector>

#include "GameObject.h"
#include "Timer.h"

using std::vector;
using std::unique_ptr;

class PlayableScene :
	public Scene
{
	std::queue<unique_ptr<GameObject>> addPendingGos;

	vector<unique_ptr<GameObject>> objects;
	unique_ptr<SceneContext> sceneContext;
	int level;

	unique_ptr<Timer> levelTimer;
	float timeLeftForLevel = 160; //seconds, originally 400 frames w/ each frame ~0.4s

public:
	int maxFireballs = 2; // Max fireballs allowed on screen at once, to prevent spamming
	explicit PlayableScene(const int level)
	{
		this->level = level;
		sceneContext = nullptr; // init on load
		levelTimer = nullptr;
	}

	void Update(float dt) override;
	void Load(const Optional<SceneSwitchContext>& ctx) override;
	void UnLoad() override;
	void Render() override;
	void CleanupDeletedObjects();
	void AddObject(GameObject* go);
	void AddObject(unique_ptr<GameObject> go);

	int GetMaxFireballs() const {
		return maxFireballs;
	}
	float GetTimeLeft() const {
		return levelTimer->GetTimeLeft();
	}
	Timer* GetLevelTimer() {
		return levelTimer.get();
	}
};
