#include "BowserFireBullet.h"
#include "Fireball.h"

BowserFireBullet::BowserFireBullet(int startX, int startY, bool isFacingRight, float targetHeight, bool heightReached, bool isAlwaysActive) : GameObject(startX, startY)
{
	auto t = Textures::GetInstance()->Get(BOWSER_ITEM_BULLET_TEX_ID);
	auto sp = Sprites::GetInstance();
	auto anims = Animations::GetInstance();
	sp->Add(BOWSER_ITEM_BULLET_FIRE_SPRITE_1, 0, 0, 23, 7, t);
	sp->Add(BOWSER_ITEM_BULLET_FIRE_SPRITE_2, 0, 8, 23, 15, t);

	if (!anims->Contains(BOWSER_ITEM_BULLET_FIRE_ANIM_ID))
	{
		auto fireBreathAnim = new Animation(100);
		fireBreathAnim->Add(BOWSER_ITEM_BULLET_FIRE_SPRITE_1);
		fireBreathAnim->Add(BOWSER_ITEM_BULLET_FIRE_SPRITE_2);
		anims->Add(BOWSER_ITEM_BULLET_FIRE_ANIM_ID, fireBreathAnim);
	}
	this->isFacingRight = isFacingRight;
	this->isAlwaysActive = isAlwaysActive;
	height = targetHeight;
	this->heightReached = heightReached;
	SetState(BowserFireBulletState::Flying);
}
void BowserFireBullet::AnimTransition()
{
	if (position.y == height)
	{
		heightReached = true;
		return;
	}
	if (!heightReached) {
		if (position.y < height)
		{
			position.y += 2.5f;
			if (position.y > height)
			{
				position.y = height;
			}
		}
		else
		{
			position.y -= 2.5f;
			if (position.y < height)
			{
				position.y = height;
			}
		}
	}
}

void BowserFireBullet::SetState(BowserFireBulletState newState)
{
	state = newState;
	switch (state)
	{
	case BowserFireBulletState::Flying:
		velocity.x = isFacingRight ? BOWSER_ITEM_FIRE_BULLET_SPEED : -BOWSER_ITEM_FIRE_BULLET_SPEED;
		isCollidable = true;
		isDeleted = false;
		break;
	case BowserFireBulletState::Discarded:
		velocity.x = 0;
		isCollidable = false;
		isDeleted = true;
		break;
	}
}

void BowserFireBullet::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (state == BowserFireBulletState::Discarded)
	{
		return;
	}
	if (!heightReached)
		AnimTransition();
	if (!isAlwaysActive)
	{
		auto r = GetBoundingBox();
		if (!Game::GetInstance()->GetCamera()->IsInView(r.left, r.top, r.right, r.bottom)) {
			SetState(BowserFireBulletState::Discarded);
			return;
		}
	}

	Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);
}

void BowserFireBullet::Render()
{
	if (state == BowserFireBulletState::Discarded)
	{
		return;
	}
	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);
	Animations::GetInstance()->Get(BOWSER_ITEM_BULLET_FIRE_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, 0);
}

void BowserFireBullet::OnNoCollision(float dt)
{
	position += velocity * dt;
}

void BowserFireBullet::OnCollisionWith(CollisionEvent* event)
{
	if (state == BowserFireBulletState::Discarded)
	{
		return;
	}
	if (event->IsTileCollision())
	{
		SetState(BowserFireBulletState::Discarded);
	}
	auto e = event->otherObject;
	if (dynamic_cast<Fireball*>(e) != nullptr)
	{
		SetState(BowserFireBulletState::Discarded);
		isDeleted = true;
	}
}