#pragma once
#include "GameObject.h"

class Mushroom : public GameObject
{
	CollectableItemState state;
	Vector2 preferPosition;
	bool moveLeft;

public:
	void SetState(CollectableItemState newState);
	explicit Mushroom(Vector2 startPos, BiomeType biome);
	void SetMoveDirX(int newDirX);
	bool IsBlocking() override { return false; }
	void Render() override;
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void OnNoCollision(float dt) override;
	void OnCollisionWith(CollisionEvent* event) override;
	Rect GetBoundingBox() override;
	int GetRenderIndex() override { return -1; }
};


