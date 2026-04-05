#pragma once
#include "GameObject.h"


enum MarioState
{
	Running,
	Idle,
};

class Mario : public GameObject
{
	bool isGrounded;

public:
	Mario(float startX, float startY);
	MarioState state;

	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	Rect GetBoundingBox() override;
	void OnNoCollision(float dt) override;
	void OnCollisionWith(CollisionEvent* event) override;
	bool IsCollidable() override { return true; }
	bool IsBlocking() override { return true; }
};

