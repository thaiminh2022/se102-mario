#pragma once
#include "Rect.h"
#include "Collision.h"
#include "GameObject.h"
#include "Scene.h"
#include "Timer.h"



enum class FireballState
{
	Bouncing,
	Exploding
};

class Fireball : public GameObject
{
	Timer lifeTimeTimer;
	Timer explodeTimer;
	SceneContext* currentContext;

	void Explode();
public:
	float ax, ay;
	bool isExploded;
	FireballState state;
	

	Fireball(float x, float y, bool isFacingRight);
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	Rect GetBoundingBox() override;
	void OnNoCollision(float dt) override;
	void OnCollisionWith(CollisionEvent* e) override;
	bool IsActive() override { return true; }
};

