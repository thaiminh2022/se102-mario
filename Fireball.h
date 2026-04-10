#pragma once
#include "Rect.h"
#include "Collision.h"
#include "GameObject.h"
#include "Scene.h"

enum class FireballState
{
	Bouncing,
	Exploding
};

class Fireball : public GameObject
{
public:
	float ax, ay;
	bool isExploded;
	FireballState state;
	const float FIREBALL_SPEED = 200.0f;  
	const float FIREBALL_GRAVITY = 900.0f;     
	const float FIREBALL_BOUNCE_SPEED = -150.0f;
	const DWORD FIREBALL_EXPLOSION_TIME = 450;
	const DWORD FIREBALL_LIFETIME = 5000;
	DWORD creationTime;
	DWORD explodeStartTime;

	Fireball(float x, float y, bool isFacingRight);
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	Rect GetBoundingBox() override;
	void OnNoCollision(float dt) override;
	void Explode();
	void OnCollisionWith(CollisionEvent* e) override;
};

