#pragma once
#include "GameObject.h"

class Bloopers : public GameObject
{
	Vector2 highestLimit;
	Vector2 lowestLimit;
	bool movingUp;

public:
	Bloopers(Vector2Int lowestLimit, Vector2Int highestLimit);
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	bool IsBlocking() override { return false; }
	void OnNoCollision(float dt) override;
	CollisionMatrixLayer GetCollisionLayer() override { return CollisionMatrixLayer::CheepCheeps; }
	Rect GetBoundingBox() override;
};
