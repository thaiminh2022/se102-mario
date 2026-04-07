#pragma once
#include "Collision.h"
#include "GameObject.h"
#include "Rect.h"
#include "Scene.h"
#include <vector>


enum class MarioState
{
	Running,
	Idle,
	Dead,
};

class Mario : public GameObject
{
	bool isGrounded;

public:
	Mario(int startX, int startY);
	MarioState state;

	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	Rect GetBoundingBox() override;
	void OnNoCollision(float dt) override;
	void OnCollisionWith(CollisionEvent* event) override;
	bool IsBlocking() override { return true; }
};

