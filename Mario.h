#pragma once
#include "GameObject.h"
#include "Rect.h"
#include "Scene.h"
#include <vector>
#include "Timer.h"


enum class MarioState : std::uint8_t
{
	Idle,
	Walking,
	Running,
	Skidding,
	Jumping,
	Ducking,
	PullingFlag,
	WalkingToCastle,
	EnteringPipe,
	ExitingPipe,
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

const float MARIO_TIME_BTW_FIRE = 0.15f;
const float MARIO_GROW_TIME = 0.7f;
const float MARIO_SHRINK_TIME = 0.75f;
const float MARIO_INVINCIBLE_TIME = 2.0f;
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

class Mario : public GameObject
{


	bool isGrounded;
	bool isInvincible;
	bool isRendering;
	static int goombaKilled;
	static int koopaKilled;
	static int coinCollected;

	float fallAcc = 562.5f;
	int GetFireBallCount(const vector<GameObject*>& coObjects) const;
	Timer fireCooldownTimer;
	Timer invincibleTimer;

	Timer transformTimer; //used for growing and shrinking

	MarioState state;
	MarioPower power;

	// flag pole interaction
	Vector2 marioWinningMoveToPosition;
	float slidingToYWinning;
	Timer flagPoleFlipWaitTimer;

	// pipe interaction
	PipeData pipeData;
	MarioPipeCtx pipeExitingData;


	void OnMarioHit();
	int GetMarioAnimId() const;
	void LoadSpriteAndAnimation();

	// on collision with
	bool OnCollisionWithGoomba(const CollisionEvent* e);
	bool OnCollisionWithKoopa(const CollisionEvent* e);
	static bool OnCollisionWithPortal(const CollisionEvent* e);
	bool OnCollisionWithQuestionBlock(const CollisionEvent* e);
	static bool OnCollisionWithCoin(const CollisionEvent* e);
	bool OnCollisionWithMushroom(const CollisionEvent* e);
	bool OnCollisionWithFlower(CollisionEvent* e);
	bool OnCollisionWithStar(const CollisionEvent* e);
	bool OnCollisionWithFlagPole(const CollisionEvent* collisionEvent);


	// update func
	void HandleGrowing(float dt);
	void HandleShrinking(float dt);
	void WhileGrounded(float dt);
	void WhileOnAir(float dt);
	void HandleJump(float dt);
	void HandleShootFireball(float dt, const vector<GameObject*>& coObjects, const SceneContext* ctx);
	void ApplyGravityAndClamp(float dt);
	void UpdateFacingDirection();
	void RouteAnimationState();
	void OnCollisionWithFireballTrap(vector<GameObject*>& coObjects);
	static int GetFlagBonusScore(float touchingHeight);

	// special states
	void MarioDyingState(float dt);
	void MarioPullingFlag(float dt);
	void MarioWalkingToCastle(float dt, vector<GameObject*>& coObjects, SceneContext* ctx);
	void MarioEnteringPipe(float dt);
	void MarioExitingPipe(float dt);
	bool CheckMarioFalloffMap();

public:
	Mario(int startX, int startY);

	MarioPower GetPowerLevel() const { return power; }
	void SetPowerLevel(const MarioPower newPower) { power = newPower; }
	
	void SetEnterPipe(const PipeData& pipe);
	void SetExitPipe(const MarioPipeCtx& returnPipeData);
	

	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	Rect GetBoundingBox() override;
	void OnNoCollision(float dt) override;
	void OnCollisionWith(CollisionEvent* event) override;
	bool IsBlocking() override { return true; }
	bool IsActive() override { return true; }
	MarioState GetState() const { return state; }
};

