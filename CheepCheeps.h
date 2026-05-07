#pragma once
#include "GameObject.h"

class CheepCheeps : public GameObject
{
	bool isRed;
public:
	CheepCheeps(Vector2Int startPos, bool isRed);
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	bool IsBlocking() override { return false; }
	void OnNoCollision(float dt) override;
	CollisionMatrixLayer GetCollisionLayer() override { return CollisionMatrixLayer::CheepCheeps; }
	Rect GetBoundingBox() override;
};

