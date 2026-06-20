#include "Bowser.h"

#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "BowserFireBullet.h"
#include "BowserHammer.h"
#include "StatManager.h"
#include "Bridge.h"
#include "Collision.h"
#include "Mario.h"
#include "Sprites.h"
#include "Textures.h"

Bowser::Bowser(Vector2Int start, Rect arena, Mario* mario) : GameObject(start), bowserArena(arena)
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

	sp->Add(BOWSER_FALL_SPRITE_1, 0, 9, 31, 40, t);
	sp->Add(BOWSER_FALL_SPRITE_2, 32, 9, 63, 40, t);


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
	if (!anims->Contains(BOWSER_DEATH_ANIM_ID))
	{
		auto deathAnim = new Animation(100);
		deathAnim->Add(BOWSER_DEATH_SPRITE_1);
		anims->Add(BOWSER_DEATH_ANIM_ID, deathAnim);
	}
	if (!anims->Contains(BOWSER_FALL_ANIM_ID))
	{
		auto fallAnim = new Animation(30);
		fallAnim->Add(BOWSER_FALL_SPRITE_1);
		fallAnim->Add(BOWSER_FALL_SPRITE_2);
		anims->Add(BOWSER_FALL_ANIM_ID, fallAnim);
	}

	nextFireBreathingTimer = Timer(BOWSER_FIRE_BREATH_INTERVAL);
	nextJumpTimer = Timer(BOWSER_JUMP_INTERVAL);
	nextHammerThrowTimer = Timer(BOWSER_HAMMERTHROW_INTERVAL);
	fallingTimer = Timer(1.5f);
	deathTimer = Timer(2.5f); // initialize but do not start

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

	BOWSER_JUMP_INTERVAL = 1 + rand() % 3; //random jump interval between 1 to 3 seconds
	BOWSER_FIRE_BREATH_INTERVAL = 3 + rand() % 3; //random fire breath interval between 3 to 5 seconds
	BOWSER_HAMMERTHROW_INTERVAL = 5 + rand() % 4; //random hammer throw interval between 5 to 8 seconds
}

void Bowser::SetState(BowserState newState)
{
	state = newState;
	switch (state)
	{
	case BowserState::Stop:
		velocity.x = 0.0f;
		velocity.y = 0.0f;
		break;
	case BowserState::Walking:
		velocity.x = moveLeft ? -BOWSER_WALKING_SPEED : BOWSER_WALKING_SPEED;
		break;
	case BowserState::Jumping:
		isGrounded = false;
		velocity.y = -BOWSER_JUMPING_SPEED;
		velocity.x = moveLeft ? -BOWSER_WALKING_SPEED : BOWSER_WALKING_SPEED;
		break;
	case BowserState::Dead:
		velocity.x = 0;
		velocity.y = 0;
		isCollidable = false;
		deathTimer = Timer(2.5f);
		deathTimer.Start();
		break;
	case BowserState::Falling:
		velocity.x = 0.0f;
		velocity.y = 0.0f;
		isCollidable = false;
		fallingTimer.Start();
		break;
	}
}
void Bowser::UpdateDirection()
{
	if (state == BowserState::Dead || state == BowserState::Falling || state == BowserState::Stop)
		return;
	if (target != nullptr && target->position.x < position.x)
	{
		isFacingRight = false;
	}
	else
	{
		isFacingRight = true;
	}
}

void Bowser::HandleHeathDecrease(int amount)
{
	health -= amount;
	if (health <= 0)
	{
		if (state != BowserState::Dead)
		{
			SetState(BowserState::Dead);
			auto sm = StatManager::GetInstance();
			sm->AddScoreWithPopup(5000, position); //only award points if Bowser died of fireballs
		}
	}
}

void Bowser::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (health <= 0 && state != BowserState::Dead)
	{
		SetState(BowserState::Dead);
	}

	if (state == BowserState::Dead)
	{
		if (deathTimer.IsTicking())
		{
			deathTimer.ProcessTimer(dt);
			if (deathTimer.IsFinished())
			{
				isDead = true;
			}
		}
	}

	UpdateDirection();
	switch (state)
	{
	case BowserState::Walking:
		velocity.x = moveLeft ? -BOWSER_WALKING_SPEED : BOWSER_WALKING_SPEED;
		break;
	case BowserState::Falling:
		fallingTimer.ProcessTimer(dt);
		if (fallingTimer.IsFinished())
		{
			SetState(BowserState::Dead); //currently kill Bowser if he falls for more than 1 second, will change later 
		}
		return;
	}
	float currentGravity = 500.0f;

	// Check if Bowser is at the "peak" of his jump (moving very slowly up or down)
	if (abs(velocity.y) < 60.0f)
	{
		// Cut gravity in half while he is hanging in the air!
		currentGravity /= 2;
	}

	velocity.y += currentGravity * dt;
	Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);
	ClampInsideArena();
	if (target->GetState() == MarioState::StopToWaitBowser && state != BowserState::Dead && state != BowserState::Falling)
		SetState(BowserState::Falling);

	if (target->GetState() == MarioState::Dying)
	{
		SetState(BowserState::Stop);
		return;
	}
	TimerHandler(dt, ctx);
	//DebugOut(L"Bowser State: %d, Health: %d, isGrounded: %d\n", static_cast<int>(state), health, isGrounded);
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
	else if (state == BowserState::Falling) {
		animId = BOWSER_FALL_ANIM_ID;
	}
	else if (state == BowserState::Walking || state == BowserState::Jumping || state == BowserState::Stop) {
		animId = BOWSER_WALK_ANIM_ID;
	}
	else
	{
		animId = BOWSER_WALK_ANIM_ID;
	}
	// override with attack animations if necessary (but only if not dead)
	if (fireBreathAnimTimer.IsTicking() && state != BowserState::Dead)
	{
		animId = BOWSER_FIRE_BREATH_ANIM_ID;
	}
	else if (hammerThrowAnimTimer.IsTicking() && state != BowserState::Dead)
	{
		animId = BOWSER_HAMMER_THROW_ANIM_ID;
		yOffset = 9.0f;
	}
	Animation* ani = Animations::GetInstance()->Get(animId);

	if (ani != nullptr)
	{
		int flipY = state == BowserState::Dead ? 1 : 0; // flip vertically if dead

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

	// --- FIRE BREATH LOGIC ---
	if (fireBreathAnimTimer.IsTicking())
	{
		// Bowser is currently in the middle of breathing fire.

		fireBreathAnimTimer.ProcessTimer(dt);
		if (fireBreathAnimTimer.IsFinished())
		{
			// Animation finished! Shoot the fireball.
			FireBreathAttack(ctx);

			// Reset the cooldown timer for the NEXT attack
			BOWSER_FIRE_BREATH_INTERVAL = static_cast<float>(3 + rand() % 3);
			nextFireBreathingTimer = Timer(BOWSER_FIRE_BREATH_INTERVAL);
			nextFireBreathingTimer.Start();
			if (isGrounded) SetState(BowserState::Walking);
		}
	}
	else
	{
		// Bowser is NOT breathing fire, so tick the cooldown timer
		nextFireBreathingTimer.ProcessTimer(dt);
		if (nextFireBreathingTimer.IsFinished())
		{
			// Start the animation timer
			fireBreathAnimTimer.Start();
			nextFireBreathingTimer.Stop();
		}
	}

	// --- HAMMER THROW LOGIC ---
	if (hammerThrowAnimTimer.IsTicking())
	{
		hammerThrowAnimTimer.ProcessTimer(dt);
		if (hammerThrowAnimTimer.IsFinished())
		{
			HammerThrowAttack(ctx);

			BOWSER_HAMMERTHROW_INTERVAL = 5 + rand() % 4;
			nextHammerThrowTimer = Timer(BOWSER_HAMMERTHROW_INTERVAL);
			nextHammerThrowTimer.Start();
		}
	}
	else
	{
		nextHammerThrowTimer.ProcessTimer(dt);
		if (nextHammerThrowTimer.IsFinished())
		{
			hammerThrowAnimTimer.Start();
			nextHammerThrowTimer.Stop();
		}
	}

	// --- JUMP LOGIC ---
	if (isGrounded)
		nextJumpTimer.ProcessTimer(dt);

	if (nextJumpTimer.IsFinished())
	{
		bool wantMoveLeft = (rand() % 2 == 0);

		if (WillJumpOutsideArena(wantMoveLeft))
		{
			wantMoveLeft = !wantMoveLeft;
		}

		moveLeft = wantMoveLeft;

		BOWSER_JUMP_INTERVAL = 1 + rand() % 3;
		nextJumpTimer = Timer(BOWSER_JUMP_INTERVAL);
		nextJumpTimer.Start();

		SetState(BowserState::Jumping);
	}
}

void Bowser::FireBreathAttack(SceneContext* ctx)
{
	if (state == BowserState::Dead || state == BowserState::Falling)
		return;
	float targetHeight = target != nullptr ? target->position.y + 4.0f : position.y;
	auto f = new BowserFireBullet(position.x, position.y, isFacingRight, targetHeight, false, false);
	ctx->addObject(f);
}

void Bowser::HammerThrowAttack(SceneContext* ctx)
{
	if (state == BowserState::Dead || state == BowserState::Falling)
		return;
	float wait = 0;
	int hammerCount = 3 + rand() % 6; // throw 3 to 8 hammers in quick succession
	for (int index = 0; index < hammerCount; index++)
	{
		wait += index * 0.03f;

		float spawnX = position.x;
		float spawnY = position.y;
		auto h = new BowserHammer(spawnX, spawnY, isFacingRight, wait);
		ctx->addObject(h);
	}
}

void Bowser::OnNoCollision(float dt)
{
	position += velocity * dt;
}

void Bowser::ResetCollisionContacts()
{
	if (state != BowserState::Dead && state != BowserState::Falling)
	{
		isGrounded = false;
	}
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
			position.y = static_cast<float>(event->otherTile->worldY + event->otherTile->tileHeight);
			if (state == BowserState::Jumping)
				SetState(BowserState::Walking);
		}
	}
	if (event->IsObjectCollision())
	{
		auto bridge = dynamic_cast<Bridge*>(event->otherObject);
		if (bridge != nullptr)
		{
			if (event->normalizedDir.y < 0 && velocity.y >= 0.0f)
			{
				SetState(BowserState::Walking);
				isGrounded = true;
			}
		}
	}
}
bool Bowser::WillJumpOutsideArena(bool movingLeft)
{
	Rect bounds = GetBoundingBox();

	float predictedDistance = 64.0f;

	float predictedLeft =
		movingLeft
		? bounds.left - predictedDistance
		: bounds.left + predictedDistance;

	float predictedRight =
		predictedLeft +
		(bounds.right - bounds.left);

	return predictedLeft < bowserArena.left + BOWSER_BOUND_PADDING || predictedRight > bowserArena.right - BOWSER_BOUND_PADDING;
}
void Bowser::ClampInsideArena()
{
	Rect bounds = GetBoundingBox();

	// LEFT
	if (bounds.left < bowserArena.left)
	{
		position.x += static_cast<float>(
			bowserArena.left - bounds.left
			);

		moveLeft = false;
		velocity.x = BOWSER_WALKING_SPEED;
	}

	// RIGHT
	if (bounds.right > bowserArena.right)
	{
		position.x -= static_cast<float>(
			bounds.right - bowserArena.right
			);

		moveLeft = true;
		velocity.x = -BOWSER_WALKING_SPEED;
	}
}
