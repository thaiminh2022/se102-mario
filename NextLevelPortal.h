#pragma once
#include "GameObject.h"

class NextLevelPortal : public GameObject
{
	Rect zone;
	int levelToLoad;
	float timeBeforeLoad;
	bool startCountdown;

public:
	NextLevelPortal(Rect zone, int levelToLoad, float timeBeforeLoad);
	void RequestNextLevel();

	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override {}
	bool IsCollidable() override { return true; }
	bool IsBlocking() override { return false; }
	Rect GetBoundingBox() override { return zone; }
};

