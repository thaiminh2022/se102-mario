#include "BowserFireBullet.h"

BowserFireBullet::BowserFireBullet(int startX, int startY, bool isFacingRight) : GameObject(startX, startY)
{
	auto t = Textures::GetInstance()->Get(BOWSER_ITEM_BULLET_TEX_ID);
	auto sp = Sprites::GetInstance();
	auto anims = Animations::GetInstance();
	sp->Add(BOWSER_ITEM_BULLET_FIRE_SPRITE_1, 0, 0, 23, 7, t);
	sp->Add(BOWSER_ITEM_BULLET_FIRE_SPRITE_2, 0, 9, 23, 16, t);

	if (!anims->Contains(BOWSER_ITEM_BULLET_FIRE_ANIM_ID))
	{
		auto fireBreathAnim = new Animation(100);
		fireBreathAnim->Add(BOWSER_ITEM_BULLET_FIRE_SPRITE_1);
		fireBreathAnim->Add(BOWSER_ITEM_BULLET_FIRE_SPRITE_2);
		anims->Add(BOWSER_ITEM_BULLET_FIRE_ANIM_ID, fireBreathAnim);
	}
	this->isFacingRight = isFacingRight;
	SetRandomHeight();
	SetState(BowserFireBulletState::Flying);
	appearTimer = Timer(2.0f);
	appearTimer.Start();

}

void BowserFireBullet::SetRandomHeight()
{
	int random = rand() % 3;
	switch (random)
	{
	case 0:
		heightLevel = BowserBulletHeight::Low;
		height = 144.0f;
		break;
	case 1:
		heightLevel = BowserBulletHeight::Average;
		height = 128.0f;
		break;
	case 2:
		heightLevel = BowserBulletHeight::High;
		height = 112.0f;
		break;
	}
}

void BowserFireBullet::AnimTransition()
{
	if (position.y == height)
	{
		return ;
	}
	else if (position.y < height)
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

void BowserFireBullet::SetState(BowserFireBulletState newState)
{
	state = newState;
	switch (state)
	{
	case BowserFireBulletState::Flying:
		velocity.x = isFacingRight ? BOWSER_ITEM_FIRE_BULLET_SPEED : -BOWSER_ITEM_FIRE_BULLET_SPEED;
		appearTimer.Start();
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
	appearTimer.ProcessTimer(dt);
	if (appearTimer.IsFinished())
	{
		SetState(BowserFireBulletState::Discarded);
		return;
	}
	AnimTransition();
	auto r = GetBoundingBox();
	if (!Game::GetInstance()->GetCamera()->IsInView(r.left, r.top, r.right, r.bottom)) {
		SetState(BowserFireBulletState::Discarded);
		return;
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
	Animations::GetInstance()->Get(BOWSER_ITEM_BULLET_FIRE_ANIM_ID)->Render(round(renderX), round(renderY),!isFacingRight,0);
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
}