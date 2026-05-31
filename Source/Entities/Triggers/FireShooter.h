#pragma once
#include "GameObject.h"
#include "Timer.h"

class FireShooter : public GameObject
{
	bool isShooting;
	Timer shootTimer;
	int blocksDistanceToShoot;
	public:
	FireShooter(int startX, int startY, Vector2Int direction);
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	void OnNoCollision(float dt) override;
	void OnCollisionWith(CollisionEvent* event) override;
	Rect GetBoundingBox() override
	{
		return Rect::FromXYWH(static_cast<int>(position.x), static_cast<int>(position.y), 16, 16);
	}
	bool IsCollidable() override { return false; }
	bool IsBlocking() override { return false; }
	CollisionMatrixLayer GetCollisionLayer() override { return CollisionMatrixLayer::Default; }
	bool IsActive() override { return true; }
};

