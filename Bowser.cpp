#include "Bowser.h"
#include "StatManager.h"

Bowser::Bowser(int startX, int startY, Mario* mario) : GameObject(startX, startY)
{
	auto t = Textures::GetInstance()->Get(BOWSER_TEX_ID);
	auto sp = Sprites::GetInstance();
	auto anims = Animations::GetInstance();

	sp->Add(BOWSER_WALK_SPRITE_1, 0, 9, 31, 40, t);
	sp->Add(BOWSER_WALK_SPRITE_2, 32, 9, 63, 40, t);

	sp->Add(BOWSER_FIRE_BREATH_SPRITE_1, 64, 9, 95, 40, t);
	sp->Add(BOWSER_FIRE_BREATH_SPRITE_2, 96, 9, 127, 40, t);

	sp->Add(BOWSER_HAMMER_THROW_SPRITE, 128, 0, 159, 40, t);

	sp->Add(BOWSER_DEATH_SPRITE_1, 32, 9, 63, 40, t); // will be vertically flipped later

	if (!anims->Contains(BOWSER_WALK_ANIM_ID))
	{
		auto walkAnim = new Animation(100);
		walkAnim->Add(BOWSER_WALK_SPRITE_1);
		walkAnim->Add(BOWSER_WALK_SPRITE_2);
		anims->Add(BOWSER_WALK_ANIM_ID, walkAnim);
	}

	if (!anims->Contains(BOWSER_FIRE_BREATH_ANIM_ID))
	{
		auto fireBreathAnim = new Animation(50);
		fireBreathAnim->Add(BOWSER_FIRE_BREATH_SPRITE_1);
		fireBreathAnim->Add(BOWSER_FIRE_BREATH_SPRITE_2);
		anims->Add(BOWSER_FIRE_BREATH_ANIM_ID, fireBreathAnim);
	}

	if (!anims->Contains(BOWSER_HAMMER_THROW_ANIM_ID))
	{
		auto hammerThrowLeftAnim = new Animation(100);
		hammerThrowLeftAnim->Add(BOWSER_HAMMER_THROW_SPRITE);
		anims->Add(BOWSER_HAMMER_THROW_ANIM_ID, hammerThrowLeftAnim);
	}

	nextFireBreathingTimer = Timer(BOWSER_FIRE_BREATH_INTERVAL);
	nextJumpTimer = Timer(BOWSER_JUMP_INTERVAL);
	nextHammerThrowTimer = Timer(BOWSER_HAMMERTHROW_INTERVAL);
	fallingTimer = Timer(1.0f);

	nextFireBreathingTimer.Start();
	nextJumpTimer.Start();
	nextHammerThrowTimer.Start();
	fireBreathAnimTimer = Timer(1.0f);
	hammerThrowAnimTimer = Timer(1.0f);

	health = 5;
	moveLeft = true;
	state = BowserState::Walking;
	target = mario;
	isGrounded = false;
	isDead = false;
}

void Bowser::SetState(BowserState newState)
{
	state = newState;
	switch (state)
	{
	case BowserState::Stop:
		velocity.x = 0;
		velocity.y = 0;
		break;
	case BowserState::Walking:
		velocity.x = moveLeft ? -BOWSER_WALKING_SPEED : BOWSER_WALKING_SPEED;
		break;
	case BowserState::Jumping:
		velocity.y = -BOWSER_JUMPING_SPEED;
		velocity.x = moveLeft ? BOWSER_WALKING_SPEED : -BOWSER_WALKING_SPEED;
		break;
	case BowserState::Dead:
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

void Bowser::HandleHeathDecrease(int amount)
{
	health -= amount;
	if (health <= 0)
	{
		SetState(BowserState::Dead);
		isDead = true;
		auto sm = StatManager::GetInstance();
		sm->AddScore(5000, position);
	}
}

void Bowser::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (health <= 0)
		isDead = true;
	if (isDead)
		return;
	UpdateDirection();
	switch (state)
	{
	case BowserState::Walking:
		velocity.x = moveLeft ? -BOWSER_WALKING_SPEED : BOWSER_WALKING_SPEED;
		break;
	case BowserState::Falling:
		fallingTimer.Start();
		fallingTimer.ProcessTimer(dt);
		if (fallingTimer.IsFinished())
		{
			SetState(BowserState::Dead); //currently kill Bowser if he falls for more than 1 second, will change later 
		}
		break;
	}
	velocity.y += 562.5f * dt;
	Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);

	if (target->GetState() == MarioState::Dying)
	{
		SetState(BowserState::Stop);
		return;
	}
	TimerHandler(dt, ctx);
}

void Bowser::Render()
{
	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);

	int animId = -1;
	float yOffset = 0;
	//walking/jumping/falling is same animation
	if (state == BowserState::Dead) {
		animId = BOWSER_DEATH_ANIM_ID;
	}
	else if (state == BowserState::Walking || state == BowserState::Jumping || state == BowserState::Falling || state == BowserState::Stop) {
		animId = BOWSER_WALK_ANIM_ID;
	}
	else
	{
		animId = BOWSER_WALK_ANIM_ID;
	}
	if (fireBreathAnimTimer.IsTicking())
	{
		animId = BOWSER_FIRE_BREATH_ANIM_ID;
	}
	else if (hammerThrowAnimTimer.IsTicking())
	{
		animId = BOWSER_HAMMER_THROW_ANIM_ID;
		yOffset = 9.0f;
	}
	Animation* ani = Animations::GetInstance()->Get(animId);

	if (ani != nullptr)
	{
		int flipY = (state == BowserState::Dead) ? 1 : 0;

		ani->Render(
			round(renderX),
			round(renderY - yOffset),
			isFacingRight,
			flipY
		);
	}
}

void Bowser::TimerHandler(float dt, SceneContext* ctx)
{
	if (state == BowserState::Dead || state == BowserState::Falling)
		return;
	nextFireBreathingTimer.ProcessTimer(dt);
	fireBreathAnimTimer.ProcessTimer(dt);
	if (nextFireBreathingTimer.IsFinished())
	{
		FireBreathAttack(ctx);
		nextFireBreathingTimer.Start();
		fireBreathAnimTimer.Start();
	}
	nextHammerThrowTimer.ProcessTimer(dt);
	hammerThrowAnimTimer.ProcessTimer(dt);
	if (nextHammerThrowTimer.IsFinished())
	{
		HammerThrowAttack(ctx);
		nextHammerThrowTimer.Start();
		hammerThrowAnimTimer.Start();
	}
	if (isGrounded)
		nextJumpTimer.ProcessTimer(dt);
	if (nextJumpTimer.IsFinished())
	{
		SetState(BowserState::Jumping);
		nextJumpTimer.Start();
	}
}

void Bowser::FireBreathAttack(SceneContext* ctx)
{
	if (state == BowserState::Dead || state == BowserState::Falling)
		return;
	auto f = new BowserFireBullet(position.x, position.y, isFacingRight);
	ctx->addObject(f);
}

void Bowser::HammerThrowAttack(SceneContext* ctx)
{
	if (state == BowserState::Dead || state == BowserState::Falling)
		return;
	float wait = 0;
	float offsetY = 0;
	for (int index = 0; index < 8; index++)
	{
		offsetY += -5.0f * index;
		wait += index * 0.03f;

		int spawnX = static_cast<int>(position.x);
		int spawnY = static_cast<int>(position.y + offsetY);
		auto h = new BowserHammer(spawnX, spawnY, isFacingRight, wait);
		ctx->addObject(h);
	}

	wait += 0.2f;
	offsetY = -20.0f;
	for (int index = 0; index < 8; index++)
	{
		offsetY += -5.0f * index;
		wait += index * 0.03f;

		int spawnX = static_cast<int>(position.x);
		int spawnY = static_cast<int>(position.y + offsetY);
		auto h = new BowserHammer(spawnX, spawnY, isFacingRight, wait);
		ctx->addObject(h);
	}
}

void Bowser::OnNoCollision(float dt)
{
	position += velocity * dt;
	isGrounded = false;
	state = BowserState::Jumping;
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
		if (event->normalizedDir.y < 0 && event->otherTile->IsBlocking() && velocity.y >= 0.0f)
		{
			SetState(BowserState::Walking);
			isGrounded = true;

		}
		else if (event->normalizedDir.y > 0 && event->otherTile->IsBlocking())
		{
			position.y = event->otherTile->worldY + event->otherTile->tileHeight;
			if (state == BowserState::Jumping)
				SetState(BowserState::Walking);
		}
	}
}