#pragma once
#include "GameObject.h"
#include "Rect.h"
#include "Scene.h"
#include <vector>
#include "Timer.h"
#include "Collision.h"
#include "GameObject.h"
#include "Scene.h"
#include "Mario.h"
#include "BowserFireBullet.h"
#include "BowserHammer.h"
#include <vector>

#include "Animation.h"
#include "Animations.h"

#include "AssetIDs.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"
#include <cmath>

#include "Debug.h"

constexpr float BOWSER_WALKING_SPEED = 15.0f;
constexpr float BOWSER_JUMPING_SPEED = 240.0f;


enum class BowserState : std::uint8_t
{
	Stop,
	Walking,
	Jumping,
	Dead,
	Falling
};
class Bowser :public GameObject
{
	bool moveLeft;
	bool isGrounded;
	bool isDead;
	Mario* target;
	int health;
	Timer nextFireBreathingTimer;
	Timer nextJumpTimer;
	Timer nextHammerThrowTimer;
	Timer fireBreathingWarmup;
	Timer fallingTimer;
	BowserState state;
	Timer fireBreathAnimTimer;
	Timer hammerThrowAnimTimer;
	float BOWSER_JUMP_INTERVAL;
	float BOWSER_FIRE_BREATH_INTERVAL;
	float BOWSER_HAMMERTHROW_INTERVAL;
public:
	Bowser(int startX, int startY, Mario* mario);
	void SetState(BowserState newState);
	BowserState GetState() const { return state;}
	int GetHealth() const { return health; }
	void HandleHeathDecrease(int amount);
	Timer GetNextFireBreathingTimer() const { return nextFireBreathingTimer; }
	void UpdateDirection();
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	void TimerHandler(float dt, SceneContext* ctx);
	void FireBreathAttack( SceneContext* ctx);
	void HammerThrowAttack(SceneContext* ctx);
	void OnNoCollision(float dt) override;
	void OnCollisionWith(CollisionEvent* event) override;
	bool IsBlocking() override { return false; }
	Rect GetBoundingBox() override
	{
		return Rect::FromXYWH(static_cast<int>(position.x), static_cast<int>(position.y), 32, 32);
	}
	bool IsActive() override { return state != BowserState::Dead; }
	CollisionMatrixLayer GetCollisionLayer() override { return CollisionMatrixLayer::Enemies; }
};

