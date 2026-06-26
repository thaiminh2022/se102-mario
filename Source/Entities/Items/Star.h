#pragma once
#include "GameObject.h"
#include "Timer.h"

class Star : public GameObject
{
	CollectableItemState state;
	Vector2 preferPos;
	Timer jumpTimer;
public:
	void SetState(CollectableItemState state);
	Star(Vector2 startPos);
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	Rect GetBoundingBox() override;
	int GetRenderIndex() override { return -1; }
	void OnNoCollision(float dt) override;
};

