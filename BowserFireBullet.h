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
#include "Bowser.h"
#include <vector>

#include "Animation.h"
#include "Animations.h"

#include "AssetIDs.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"
#include <cmath>

#include "Debug.h"

#define BOWSER_ITEM_FIRE_BULLET_SPEED 100.0f

enum class BowserFireBulletState : std::uint8_t
{
	Flying,
	Discarded
};

class BowserFireBullet :public GameObject
{
	float height; // the y position of Mario when the FireBullet is shot
	bool heightReached;
	bool isAlwaysActive; // even when off screen, the fire bullet will still update its position until it reaches the target height, after which it will only update when on screen
	BowserFireBulletState state;
public:
	BowserFireBullet(int startX, int startY, bool isFacingRight, float targetHeight, bool heightReached, bool isAlwaysActive = false);
	void SetRandomHeight();
	void SetState(BowserFireBulletState newState);
	BowserFireBulletState GetState() const { return state; }
	void AnimTransition();
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;

	void Render() override;
	void OnNoCollision(float dt) override;
	void OnCollisionWith(CollisionEvent* event) override;
	Rect GetBoundingBox() override
	{
		return Rect::FromXYWH(static_cast<int>(position.x), static_cast<int>(position.y), 24, 8);
	}
	CollisionMatrixLayer GetCollisionLayer() override { return CollisionMatrixLayer::EnemyProjectile; }
	bool IsActive() override { return true; }
};

