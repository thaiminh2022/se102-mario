#pragma once
#include "GameObject.h"
#include "Rect.h"
#include "Scene.h"
#include <vector>
#include "Timer.h"

#include "Game.h"

#include <cmath>


constexpr float BOWSER_WALKING_SPEED = 15.0f;
constexpr float BOWSER_JUMPING_SPEED = 240.0f;
constexpr float BOWSER_BOUND_PADDING = 8.0f;


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
	Timer deathTimer; 
	BowserState state;
	Timer fireBreathAnimTimer;
	Timer hammerThrowAnimTimer;
	float BOWSER_JUMP_INTERVAL;
	float BOWSER_FIRE_BREATH_INTERVAL;
	float BOWSER_HAMMERTHROW_INTERVAL;
	Rect bowserArena;
public:
	Bowser(Vector2Int start, Rect arena, Mario* mario);
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
	void ResetCollisionContacts() override;
	void OnNoCollision(float dt) override;
	bool WillJumpOutsideArena(bool movingLeft);
	void ClampInsideArena();
	void OnCollisionWith(CollisionEvent* event) override;
	bool IsBlocking() override { return false; }
	Rect GetBoundingBox() override
	{
		return Rect::FromXYWH(static_cast<int>(position.x), static_cast<int>(position.y), 32, 32);
	}
	CollisionMatrixLayer GetCollisionLayer() override { return CollisionMatrixLayer::Enemies; }
	void SetBowserArena(const Rect& arena) { bowserArena = arena; }
};

