#include "Bowser.h"

Bowser::Bowser(int startX, int startY,Mario *mario) : GameObject(startX, startY)
{
	auto t = Textures::GetInstance()->Get(BOWSER_TEX_ID);
	auto sp = Sprites::GetInstance();
	auto anims = Animations::GetInstance();

	sp->Add(BOWSER_WALK_SPRITE_1, 0, 9, 31, 40, t);
	sp->Add(BOWSER_WALK_SPRITE_2, 34, 9, 65, 40, t);

	sp->Add(BOWSER_JUMP_SPRITE_1, 0, 9, 31, 40, t);

	sp->Add(BOWSER_FIRE_BREATH_SPRITE_1, 68, 9, 99, 40, t);
	sp->Add(BOWSER_FIRE_BREATH_SPRITE_2, 102, 9, 133, 40, t);

	sp->Add(BOWSER_HAMMER_THROW_LEFT_SPRITE, 136, 0, 167, 40, t);
	sp->Add(BOWSER_HAMMER_THROW_RIGHT_SPRITE, 170, 0, 201, 40, t);

	sp->Add(BOWSER_DEATH_SPRITE_1, 0, 9, 31, 40, t);

	sp->Add(BOWSER_FALL_SPRITE_1, 0, 9, 31, 40, t);
	sp->Add(BOWSER_FALL_SPRITE_2, 34, 9, 65, 40, t);

	if (!anims->Contains(BOWSER_WALK_ANIM_ID))
	{
		auto walkAnim = new Animation(100);
		walkAnim->Add(BOWSER_WALK_SPRITE_1);
		walkAnim->Add(BOWSER_WALK_SPRITE_2);
		anims->Add(BOWSER_WALK_ANIM_ID, walkAnim);
	}

	if (!anims->Contains(BOWSER_JUMP_ANIM_ID))
	{
		auto jumpAnim = new Animation(100);
		jumpAnim->Add(BOWSER_JUMP_SPRITE_1);
		anims->Add(BOWSER_JUMP_ANIM_ID, jumpAnim);
	}

	if (!anims->Contains(BOWSER_FIRE_BREATH_ANIM_ID))
	{
		auto fireBreathAnim = new Animation(100);
		fireBreathAnim->Add(BOWSER_FIRE_BREATH_SPRITE_1);
		fireBreathAnim->Add(BOWSER_FIRE_BREATH_SPRITE_2);
		anims->Add(BOWSER_FIRE_BREATH_ANIM_ID, fireBreathAnim);
	}

	if (!anims->Contains(BOWSER_HAMMER_THROW_LEFT_ANIM_ID))
	{
		auto hammerThrowLeftAnim = new Animation(100);
		hammerThrowLeftAnim->Add(BOWSER_HAMMER_THROW_LEFT_SPRITE);
		anims->Add(BOWSER_HAMMER_THROW_LEFT_ANIM_ID, hammerThrowLeftAnim);
	}

	if (!anims->Contains(BOWSER_HAMMER_THROW_RIGHT_ANIM_ID))
	{
		auto hammerThrowRightAnim = new Animation(100);
		hammerThrowRightAnim->Add(BOWSER_HAMMER_THROW_RIGHT_SPRITE);
		anims->Add(BOWSER_HAMMER_THROW_RIGHT_ANIM_ID, hammerThrowRightAnim);
	}

	if (!anims->Contains(BOWSER_DEATH_ANIM_ID))
	{
		auto deathAnim = new Animation(100);
		deathAnim->Add(BOWSER_DEATH_SPRITE_1);
		anims->Add(BOWSER_DEATH_ANIM_ID, deathAnim);
	}

	if (!anims->Contains(BOWSER_FALL_ANIM_ID))
	{
		auto fallAnim = new Animation(10);
		fallAnim->Add(BOWSER_FALL_SPRITE_1);
		fallAnim->Add(BOWSER_FALL_SPRITE_2);
		anims->Add(BOWSER_FALL_ANIM_ID, fallAnim);
	}

	nextFireBreathingTimer = Timer(2.0f);
	nextHammerThrowingTimer = Timer(3.0f);
	nextJumpTimer = Timer(3.0f);
	fallingTimer = Timer(1.0f);
	
	nextFireBreathingTimer.Start();
	nextHammerThrowingTimer.Start();
	nextJumpTimer.Start();
	fireBreathAnimTimer = Timer(0.5f);
	hammerThrowAnimTimer = Timer(1.0f);

	moveLeft = true;
	state = BowserState::Walking;
	target = mario;

	isFireBreathing = false;
	isHammerThrowing = false;
}

void Bowser::SetState(BowserState newState)
{
	state = newState;
	switch (state)
	{
	case BowserState::Stop:
		velocity.x = 0;
		break;
	case BowserState::Walking:
		velocity.x = moveLeft ? -BOWSER_WALKING_SPEED : BOWSER_WALKING_SPEED;
		break;
	case BowserState::Jumping:
		velocity.y = -BOWSER_JUMPING_SPEED;
		break;
	case BowserState::Dead:
		velocity.x = 0;
		velocity.y = 0;
		isCollidable = false;
		isDeleted = true;
		break;
	case BowserState::Falling:
		velocity.x = 0;
		velocity.y = 0;
		isCollidable = false;
		break;
	}
}
void Bowser::UpdateDirection()
{
	if (state == BowserState::Dead || state == BowserState::Falling || state == BowserState::Stop)
		return;
	if (target->position.x < position.x)
	{
		isFacingRight = false;
		moveLeft = true;
	}
	else
	{
		isFacingRight = true;
		moveLeft = false;
	}
}

void Bowser::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) 
{
	UpdateDirection();
	switch (state)
	{
	case BowserState::Walking:
		velocity.x = moveLeft ? -BOWSER_WALKING_SPEED : BOWSER_WALKING_SPEED;
		break;
	case BowserState::Jumping:
		velocity.y -= BOWSER_JUMPING_SPEED * dt;
		break;
	case BowserState::Falling:
		fallingTimer.Start();
		fallingTimer.ProcessTimer(dt);
		if (fallingTimer.IsFinished())
		{
			SetState(BowserState::Dead);
		}
	case BowserState::Dead:
		velocity.y += 500.0f * dt;
		break;
	}
	velocity.y += 500.0f * dt;
	Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);

	if (target->GetState() == MarioState::Dying)
	{
		SetState(BowserState::Stop);
		return;
	}
	TimerHandler(dt, coObjects, ctx);
}

void Bowser::Render()
{
	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);

	if (state == BowserState::Falling)
	{
		Animations::GetInstance()->Get(BOWSER_FALL_ANIM_ID)
			->Render(round(renderX), round(renderY), isFacingRight, 0);
	}
	else if (isFireBreathing)
	{
		Animations::GetInstance()->Get(BOWSER_FIRE_BREATH_ANIM_ID)
			->Render(round(renderX), round(renderY), isFacingRight, 0);
	}
	else if(isHammerThrowing)
	{
		Animations::GetInstance()
			->Get(isFacingRight ? BOWSER_HAMMER_THROW_RIGHT_ANIM_ID : BOWSER_HAMMER_THROW_LEFT_ANIM_ID)
			->Render(round(renderX), round(renderY) ,0, 0);
	}
	else
	{
		Animations::GetInstance()
			->Get(state == BowserState::Dead ? BOWSER_DEATH_ANIM_ID :
				(state == BowserState::Jumping ? BOWSER_JUMP_ANIM_ID: BOWSER_WALK_ANIM_ID))
			->Render(round(renderX), round(renderY), isFacingRight, state == BowserState::Dead ? 1 : 0);
	}
}

void Bowser::TimerHandler(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (state == BowserState::Dead || state == BowserState::Falling)
		return;
	nextFireBreathingTimer.ProcessTimer(dt);
	if (nextFireBreathingTimer.IsFinished())
	{
		isFireBreathing = true;
		nextFireBreathingTimer.Start();
		fireBreathAnimTimer.Start();
	}

	nextHammerThrowingTimer.ProcessTimer(dt);
	if (nextHammerThrowingTimer.IsFinished())
	{
		isHammerThrowing = true;
		for (int i = 0; i < 5; i++)
			HammerThrowAttack(i, coObjects, ctx);
		nextHammerThrowingTimer.Start();
		hammerThrowAnimTimer.Start();
	}

	nextJumpTimer.ProcessTimer(dt);
	if (nextJumpTimer.IsFinished())
	{
		SetState(BowserState::Jumping);
		nextJumpTimer.Start();
	}

	if (isFireBreathing)
	{
		fireBreathAnimTimer.ProcessTimer(dt);
		if (fireBreathAnimTimer.IsFinished())
		{
			isFireBreathing = false;
			FireBreathAttack(coObjects, ctx);
			fireBreathAnimTimer.SetIdle();
		}
	}

	if (isHammerThrowing)
	{
		hammerThrowAnimTimer.ProcessTimer(dt);
		if (hammerThrowAnimTimer.IsFinished())
		{
			isHammerThrowing = false;
			hammerThrowAnimTimer.SetIdle();
		}
	}
}

void Bowser::FireBreathAttack(vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (state == BowserState::Dead || state == BowserState::Falling)
		return;
	auto f = new BowserFireBullet(position.x, position.y, isFacingRight);
	ctx->addObject(f);
}

// changed: accept an index to stagger and offset spawn
void Bowser::HammerThrowAttack(int index, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (state == BowserState::Dead || state == BowserState::Falling)
		return;

	float dir = isFacingRight ? 1.0f : -1.0f;
	float offsetY = -10.0f - index ;
	float wait = index * 0.16f;

	int spawnX = static_cast<int>(position.x );
	int spawnY = static_cast<int>(position.y + offsetY);

	auto h = new BowserHammer(spawnX, spawnY, isFacingRight, wait);
	ctx->addObject(h);
}

void Bowser::OnNoCollision(float dt)
{
	position += velocity * dt;
}

void Bowser::OnCollisionWith(CollisionEvent* event)
{
	if (state == BowserState::Dead || state == BowserState::Falling)
		return;
	if (event->IsTileCollision())
	{
		if (event->otherTile->type == CollisionTileType::Death)
		{
			SetState(BowserState::Falling);
			return;
		}
		if (event->normalizedDir.x != 0 && event->otherTile->IsBlocking())
		{
			moveLeft = !moveLeft;
		}
		if (event->normalizedDir.y < 0 && event->otherTile->IsBlocking())
		{
			position.y = event->otherTile->worldY - GetBoundingBox().GetHeight();
			SetState(BowserState::Walking);
		}
		else if (event->normalizedDir.y > 0 && event->otherTile->IsBlocking())
		{
			position.y = event->otherTile->worldY + event->otherTile->tileHeight;
			if (state == BowserState::Jumping)
				SetState(BowserState::Walking);
		}
	}
}