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
constexpr float BOWSER_JUMPING_SPEED = 200.0f;

enum class BowserState : std::uint8_t
{
	Walking,
	Jumping,
	Dead,
	Falling
};

class Bowser :public GameObject
{
	bool moveLeft;
	Mario* target;
	Timer nextFireBreathingTimer;
	Timer nextHammerThrowingTimer;
	Timer nextJumpTimer;
	Timer fallingTimer;
	BowserState state;
	bool isFireBreathing;
	bool isHammerThrowing;
	Timer fireBreathAnimTimer;
	Timer hammerThrowAnimTimer;
public:
	Bowser(int startX, int startY, Mario* mario);
	void SetState(BowserState newState);
	BowserState GetState() const { return state;}
	Timer GetNextFireBreathingTimer() const { return nextFireBreathingTimer; }
	Timer GetNextHammerThrowingTimer() const { return nextHammerThrowingTimer; }
	void UpdateDirection();
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	void TimerHandler(float dt, vector<GameObject*>& coObjects, SceneContext* ctx);
	void FireBreathAttack(vector<GameObject*>& coObjects, SceneContext* ctx);
	void HammerThrowAttack(int index, vector<GameObject*>& coObjects, SceneContext* ctx);
	void OnNoCollision(float dt) override;
	void OnCollisionWith(CollisionEvent* event) override;
	Rect GetBoundingBox() override
	{
		return Rect::FromXYWH(static_cast<int>(position.x), static_cast<int>(position.y), 32, 32);
	}
};

