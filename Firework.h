#pragma once
#include "GameObject.h"
#include "Timer.h"

class Firework : public GameObject
{
	Timer lifeTimer;

	static void EnsureAssets();

public:
	explicit Firework(Vector2 position);
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	Rect GetBoundingBox() override;
	bool IsCollidable() override { return false; }
	bool IsBlocking() override { return false; }
	int GetRenderIndex() override { return 2; }
};
