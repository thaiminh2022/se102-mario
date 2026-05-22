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
#include <vector>

#include "Animation.h"
#include "Animations.h"

#include "AssetIDs.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"
#include <cmath>

#include "Debug.h"

#define BOWSER_ITEM_FIRE_HAMMER_SPEED 120.0f

enum class BowserHammerState : std::uint8_t
{
	Waiting,
    Flying,
	Discarded
};
class BowserHammer :public GameObject
{ 	
	BowserHammerState state;
	Timer waitTimer;
public:
	BowserHammer(int startX, int startY, bool isFacingRight, float waitTime);
	void SetState(BowserHammerState newState);
	void SetRandomVelocity();
	bool IsBlocking() override { return false; }
	BowserHammerState GetState() const { return state; }
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	void OnNoCollision(float dt) override;
	void OnCollisionWith(CollisionEvent* event) override;
	
	Rect GetBoundingBox() override
	{
		return Rect::FromXYWH(static_cast<int>(position.x) + 8, static_cast<int>(position.y) + 8, 8, 8);
	}
	CollisionMatrixLayer GetCollisionLayer() override { return CollisionMatrixLayer::EnemyProjectile; }
};

