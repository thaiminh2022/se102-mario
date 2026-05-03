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

// The height of the bullet determines the y offset from Bowser's position when the bullet is spawned. This is used to create a more dynamic attack pattern for Bowser.
enum class BowserBulletHeight : std::uint8_t
{
	Low,
	Average,
	High
};

class BowserFireBullet :public GameObject
{
	BowserBulletHeight height;
	BowserFireBulletState state;
	Timer appearTimer;
public:
	BowserFireBullet(int startX, int startY, bool isFacingRight);
	void SetRandomHeight();
	void SetState(BowserFireBulletState newState);
	BowserFireBulletState GetState() const { return state; }
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;

	void Render() override;
	void OnNoCollision(float dt) override;
	void OnCollisionWith(CollisionEvent* event) override;
	Rect GetBoundingBox() override
	{
		return Rect::FromXYWH(static_cast<int>(position.x), static_cast<int>(position.y), 24, 8);
	}
};

