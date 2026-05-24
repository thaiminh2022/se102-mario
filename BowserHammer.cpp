#include "BowserHammer.h"

#include "Animations.h"
#include "AssetIDs.h"
#include "Fireball.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"

BowserHammer::BowserHammer(float startX, float startY, bool isFacingRight, float waitTime) : GameObject(startX, startY), waitTimer(waitTime)
{
	auto t = Textures::GetInstance()->Get(BOWSER_ITEM_BULLET_TEX_ID);
	auto sp = Sprites::GetInstance();
	auto anims = Animations::GetInstance();
	sp->Add(BOWSER_ITEM_BULLET_HAMMER_SPRITE_1, 24, 0, 47, 23, t);
	sp->Add(BOWSER_ITEM_BULLET_HAMMER_SPRITE_2, 48, 0, 71, 23, t);
	sp->Add(BOWSER_ITEM_BULLET_HAMMER_SPRITE_3, 72, 0, 95, 23, t);
	sp->Add(BOWSER_ITEM_BULLET_HAMMER_SPRITE_4, 96, 0, 119, 23, t);

	if (!anims->Contains(BOWSER_ITEM_BULLET_HAMMER_ANIM_ID))
	{
		auto hammerAnim = new Animation(100);
		hammerAnim->Add(BOWSER_ITEM_BULLET_HAMMER_SPRITE_1);
		hammerAnim->Add(BOWSER_ITEM_BULLET_HAMMER_SPRITE_2);
		hammerAnim->Add(BOWSER_ITEM_BULLET_HAMMER_SPRITE_3);
		hammerAnim->Add(BOWSER_ITEM_BULLET_HAMMER_SPRITE_4);
		anims->Add(BOWSER_ITEM_BULLET_HAMMER_ANIM_ID, hammerAnim);
	}
	this->isFacingRight = isFacingRight;
	SetState(BowserHammerState::Waiting);
	this->waitTimer = Timer(waitTime);
	waitTimer.Start();
}

void BowserHammer::SetState(BowserHammerState newState)
{
	state = newState;
	switch (state)
	{
	case BowserHammerState::Flying:
		velocity.y = -200.0f;
		velocity.x = isFacingRight ? BOWSER_ITEM_FIRE_HAMMER_SPEED : -BOWSER_ITEM_FIRE_HAMMER_SPEED;
		isCollidable = true;
		isDeleted = false;
		break;
	case BowserHammerState::Discarded:
		velocity.x = 0.0f;
		isCollidable = false;
		isDeleted = true;
		break;
	}
}

void BowserHammer::SetRandomVelocity()
{
	int randomX = rand() % 70 + 80; // 80 to 150
	int randomY = rand() % 50 + 250; // 250 to 300
	velocity.x = static_cast<float>(isFacingRight ? randomX : -randomX);
	velocity.y = static_cast<float>(-randomY);
}

void BowserHammer::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	waitTimer.ProcessTimer(dt);
	if (state == BowserHammerState::Waiting)
	{
		if (waitTimer.IsFinished())
		{
			SetState(BowserHammerState::Flying);
		}
		return;
	}
	if (state == BowserHammerState::Discarded)
		return;

	velocity.y += 500.0f * dt;
	auto r = GetBoundingBox();
	if (!Game::GetInstance()->GetCamera()->IsInView(r.left, r.top, r.right, r.bottom)) {
		SetState(BowserHammerState::Discarded);
		return;
	}
	 Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);
}

void BowserHammer::Render()
{
	if (state == BowserHammerState::Discarded || state == BowserHammerState::Waiting)
		return;
	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);
	Animations::GetInstance()->Get(BOWSER_ITEM_BULLET_HAMMER_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, 0);
}

void BowserHammer::OnNoCollision(float dt)
{
	if (state == BowserHammerState::Waiting)
		return;
	position += velocity * dt;
}

void BowserHammer::OnCollisionWith(CollisionEvent* event)
{
	if (state == BowserHammerState::Waiting || state == BowserHammerState::Discarded)
		return;
	auto e = event->otherObject;
	if (dynamic_cast<Fireball*>(e) != nullptr)
	{
		SetState(BowserHammerState::Discarded);
		isDeleted = true;
	}
}
