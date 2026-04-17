#pragma once
#include "GameObject.h"
#include "Rect.h"
#include "Scene.h"
#include <vector>

#include "Timer.h"

// A random guy on youtube got these numbers
const float MIN_WALK = 4.453125f; // Minimum speed to be considered walking, otherwise it's idle
const float MAX_WALK = 93.75f;

const float MAX_RUN = 153.75f;

//ACCELERATION
const float ACC_WALK = 133.59375f;
const float ACC_RUN = 200.390625f;

//Deceleration when no input
const float DEC_REL = 182.8125f;
//Deceleration when changing direction
const float DEC_SKID = 365.625f;


//Fall acceleration when not holding jump
const float STOP_FALL = 1575.0f;
const float WALK_FALL = 1800.0f;
const float RUN_FALL = 2025.0f;

//Fall acceleration when holding jump
const float STOP_FALL_A = 450.0f;
const float WALK_FALL_A = 421.875f;
const float RUN_FALL_A = 562.5f;

const float MAX_FALL = 270.0f;
const int MAX_FIREBALL_COUNT = 2;
const float MARIO_TIME_BTW_FIRE = 0.15f;
const float MARIO_TRANSFORM_TIME = 1.0f;

enum class MarioState : std::uint8_t
{
	Idle,
	Walking,
	Running,
	Skidding,
	Jumping,
	Ducking,
	PullingFlag,
	Dying,
	Firing,
	Growing,
	Shrinking
};

enum class MarioPower
{
	Normal,
	Big,
	Fire
};


class Mario : public GameObject
{
	bool isGrounded;
	static int goombaKilled;
	static int coinCollected;

	float fallAcc = 562.5f;
	int GetFireBallCount(const vector<GameObject*>& coObjects) const;
	Timer fireCooldownTimer;
	Timer transformTimer;

	MarioState state;
	MarioPower power;

	void OnMarioHit();

public:
	Mario(int startX, int startY);

	MarioPower GetPowerLevel() const { return power; }
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	Rect GetBoundingBox() override;
	void OnNoCollision(float dt) override;
	void OnCollisionWith(CollisionEvent* event) override;
	bool IsBlocking() override { return true; }
};

