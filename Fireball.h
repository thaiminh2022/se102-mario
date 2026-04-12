#pragma once
#include "Rect.h"
#include "Collision.h"
#include "GameObject.h"
#include "Scene.h"
#include "Timer.h"

const float FIREBALL_SPEED = 200.0f;
const float FIREBALL_GRAVITY = 900.0f;
const float FIREBALL_BOUNCE_SPEED = -150.0f;
const float FIREBALL_EXPLOSION_TIME = 0.45f;
const float FIREBALL_LIFETIME = 5.0f;

enum class FireballState
{
	Bouncing,
	Exploding
};

class Fireball : public GameObject
{
	Timer lifeTimeTimer;
	Timer explodeTimer;

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
};

