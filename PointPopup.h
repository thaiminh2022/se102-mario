#pragma once
#include "GameObject.h"
#include "Timer.h"

class PointPopup : public GameObject
{
	int animId;
	Timer lifeTimer;
	float riseSpeed;

	static void EnsureAssets();
	static int GetAnimIdForScore(int scoreValue);

public:
	PointPopup(Vector2 pos, int scoreValue);
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	Rect GetBoundingBox() override;
	bool IsCollidable() override { return false; }
	bool IsBlocking() override { return false; }
	int GetRenderIndex() override { return -2; }
};
