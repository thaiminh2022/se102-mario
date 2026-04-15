#include "Mario.h"

#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "Collision.h"
#include "Game.h"
#include "GameObject.h"
#include "Goomba.h"
#include "InputManager.h"
#include "Rect.h"
#include "Scene.h"
#include "Sprites.h"
#include "Textures.h"
#include <algorithm>
#include <vector>

#include <cmath>

#include "Debug.h"
#include "AudioManager.h"
#include "Coin.h"
#include "FontManager.h"
#include "Mushroom.h"
#include "NextLevelPortal.h"
#include "QuestionBlock.h"
#include "Fireball.h"
#include "Flower.h"

int Mario::goombaKilled = 0;
int Mario::coinCollected = 0;


int Mario::GetFireBallCount(const vector<GameObject*>& coObjects) const
{
	int count = 0;

	for (const auto& co : coObjects)
	{
		if (GameObject::IsDeleted(co))
			continue;

		if (dynamic_cast<Fireball*>(co) != nullptr)
		{
			count++;
		}
	}
	return count;
}

Mario::Mario(int startX, int startY) : GameObject(static_cast<float>(startX), static_cast<float>(startY))

{
	auto marioTex = Textures::GetInstance()->Get(MARIO_TEX_ID);

	auto anims = Animations::GetInstance();
	auto sprites = Sprites::GetInstance();

	/// ================================
	// Normal sprites
	/// ================================
	sprites->Add(MARIO_IDLE_SPRITE_1, 0, 0, 15, 15, marioTex);

	sprites->Add(MARIO_RUN_SPRITE_1, 16, 0, 31, 15, marioTex);
	sprites->Add(MARIO_RUN_SPRITE_2, 32, 0, 47, 15, marioTex);
	sprites->Add(MARIO_RUN_SPRITE_3, 48, 0, 63, 15, marioTex);

	sprites->Add(MARIO_SKID_SPRITE_1, 64, 0, 79, 15, marioTex);

	sprites->Add(MARIO_JUMP_SPRITE_1, 80, 0, 95, 15, marioTex);

	sprites->Add(MARIO_DEATH_SPRITE_1, 96, 0, 111, 15, marioTex);

	sprites->Add(MARIO_GROW_SPRITE_1, 0, 48, 15, 79, marioTex);
	sprites->Add(MARIO_GROW_SPRITE_2, 16, 48, 31, 79, marioTex);
	sprites->Add(MARIO_GROW_SPRITE_3, 0, 16, 15, 47, marioTex);

	// idle anim
	Animation* anim = new Animation(300);
	anim->Add(MARIO_IDLE_SPRITE_1);
	anims->Add(MARIO_IDLE_ANIM_ID, anim);

	// walk anim
	anim = new Animation(100);
	anim->Add(MARIO_RUN_SPRITE_1);
	anim->Add(MARIO_RUN_SPRITE_2);
	anim->Add(MARIO_RUN_SPRITE_3);
	anims->Add(MARIO_RUN_ANIM_ID, anim);

	// skid anim
	anim = new Animation(100);
	anim->Add(MARIO_SKID_SPRITE_1);
	anims->Add(MARIO_SKID_ANIM_ID, anim);

	// jump anim
	anim = new Animation(100);
	anim->Add(MARIO_JUMP_SPRITE_1);
	anims->Add(MARIO_JUMP_ANIM_ID, anim);

	// death anim
	anim = new Animation(100);
	anim->Add(MARIO_DEATH_SPRITE_1);
	anims->Add(MARIO_DEATH_ANIM_ID, anim);

	// grow anim
	anim = new Animation(300);
	anim->Add(MARIO_GROW_SPRITE_1, 100);
	anim->Add(MARIO_GROW_SPRITE_2, 500);
	anim->Add(MARIO_GROW_SPRITE_3, 100);
	anims->Add(MARIO_GROW_ANIM_ID, anim);

	/// ================================
	// BIG sprites
	/// ================================
	sprites->Add(MARIO_BIG_IDLE_SPRITE_1, 0, 16, 15, 47, marioTex);

	sprites->Add(MARIO_BIG_RUN_SPRITE_1, 16, 16, 31, 47, marioTex);
	sprites->Add(MARIO_BIG_RUN_SPRITE_2, 32, 16, 47, 47, marioTex);
	sprites->Add(MARIO_BIG_RUN_SPRITE_3, 48, 16, 63, 47, marioTex);
	
	sprites->Add(MARIO_BIG_SKID_SPRITE_1, 64, 16, 79, 47, marioTex);

	sprites->Add(MARIO_BIG_JUMP_SPRITE_1, 80, 16, 95, 47, marioTex);

	sprites->Add(MARIO_BIG_DUCK_SPRITE_1, 96, 16, 111, 47, marioTex);

	sprites->Add(MARIO_BIG_SHRINK_SPRITE_1, 32, 48, 47, 79, marioTex);
	sprites->Add(MARIO_BIG_SHRINK_SPRITE_2, 48, 48, 63, 79, marioTex);


	anim = new Animation(100);
	// idle anim
	anim->Add(MARIO_BIG_IDLE_SPRITE_1);
	anims->Add(MARIO_BIG_IDLE_ANIM_ID, anim);

	// walk anim
	anim = new Animation(100);
	anim->Add(MARIO_BIG_RUN_SPRITE_1);
	anim->Add(MARIO_BIG_RUN_SPRITE_2);
	anim->Add(MARIO_BIG_RUN_SPRITE_3);
	anims->Add(MARIO_BIG_RUN_ANIM_ID, anim);

	// skid anim
	anim = new Animation(100);
	anim->Add(MARIO_BIG_SKID_SPRITE_1);
	anims->Add(MARIO_BIG_SKID_ANIM_ID, anim);

	// jump anim
	anim = new Animation(100);
	anim->Add(MARIO_BIG_JUMP_SPRITE_1);
	anims->Add(MARIO_BIG_JUMP_ANIM_ID, anim);

	// duck anim
	anim = new Animation(100);
	anim->Add(MARIO_BIG_DUCK_SPRITE_1);
	anims->Add(MARIO_BIG_DUCK_ANIM_ID, anim);

	//shrink anim
	anim = new Animation(100);
	anim->Add(MARIO_BIG_SHRINK_SPRITE_1, 300);
	anim->Add(MARIO_BIG_SHRINK_SPRITE_2, 300);


	/// ================================
	// FIRE sprites
	/// ================================
	sprites->Add(MARIO_FIRE_IDLE_SPRITE_1, 0, 80, 15, 111, marioTex);

	sprites->Add(MARIO_FIRE_RUN_SPRITE_1, 16, 80, 31, 111, marioTex);
	sprites->Add(MARIO_FIRE_RUN_SPRITE_2, 32, 80, 47, 111, marioTex);
	sprites->Add(MARIO_FIRE_RUN_SPRITE_3, 48, 80, 63, 111, marioTex);
	sprites->Add(MARIO_FIRE_SKID_SPRITE_1, 64, 80, 79, 111, marioTex);

	sprites->Add(MARIO_FIRE_JUMP_SPRITE_1, 80, 80, 95, 111, marioTex);
	sprites->Add(MARIO_FIRE_DUCK_SPRITE_1, 96, 80, 111, 111, marioTex);

	sprites->Add(MARIO_FIRE_FIRE_SPRITE_1, 16, 80, 31, 111, marioTex);// Reusing the run sprite for firing since it's the same pose

	sprites->Add(MARIO_FIRE_SHRINK_SPRITE_1, 80, 48, 95, 79, marioTex);
	sprites->Add(MARIO_FIRE_SHRINK_SPRITE_2, 96, 48, 111, 79, marioTex);

	//idle anim
	anim = new Animation(300);
	anim->Add(MARIO_FIRE_IDLE_SPRITE_1);
	anims->Add(MARIO_FIRE_IDLE_ANIM_ID, anim);

	// walk anim
	anim = new Animation(100);
	anim->Add(MARIO_FIRE_RUN_SPRITE_1);
	anim->Add(MARIO_FIRE_RUN_SPRITE_2);
	anim->Add(MARIO_FIRE_RUN_SPRITE_3);
	anims->Add(MARIO_FIRE_RUN_ANIM_ID, anim);

	// skid anim
	anim = new Animation(100);
	anim->Add(MARIO_FIRE_SKID_SPRITE_1);
	anims->Add(MARIO_FIRE_SKID_ANIM_ID, anim);

	// jump anim
	anim = new Animation(100);
	anim->Add(MARIO_FIRE_JUMP_SPRITE_1);
	anims->Add(MARIO_FIRE_JUMP_ANIM_ID, anim);

	// duck anim
	anim = new Animation(100);
	anim->Add(MARIO_FIRE_DUCK_SPRITE_1);
	anims->Add(MARIO_FIRE_DUCK_ANIM_ID, anim);

	// fire anim
	anim = new Animation(300);
	anim->Add(MARIO_FIRE_FIRE_SPRITE_1);
	anims->Add(MARIO_FIRE_FIRE_ANIM_ID, anim);

	// shrink anim
	anim = new Animation(100);
	anim->Add(MARIO_FIRE_SHRINK_SPRITE_1, 300);
	anim->Add(MARIO_FIRE_SHRINK_SPRITE_2, 300);

	isGrounded = false;
	isCollidable = true;
	isFacingRight = true;
	velocity.x = 0.0f;
	velocity.y = 0.0f;
	state = MarioState::Idle;
	power = MarioPower::Fire;
	fireCooldownTimer = Timer(MARIO_TIME_BTW_FIRE);
	fireCooldownTimer.Start();
	// manually change power here for testing, will be changed in the future when we implement power-ups

}

void Mario::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (state == MarioState::Dying)
	{
		// dead animation for now
		Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);
		velocity.y = 9000.0f * dt;
		velocity.x = 0;
		return;
	}
	if (state == MarioState::Growing) {
		transformTimer -= dt * 1000;
		if (transformTimer > 0)
			return;
		else {
			power = MarioPower::Big;
			if (isGrounded)
				state = MarioState::Idle;
			else
				state = MarioState::Jumping;
		}
	}
	auto g = Game::GetInstance();
	/*g->DrawDebugRect(GetBoundingBox(), D3DXCOLOR(1, 0, 0, 1));*/
	auto input = InputManager::GetInstance();
	if (isGrounded)
	{
		if (input->IsKeyDown('S') && (power == MarioPower::Big || power == MarioPower::Fire)) {
			// Ducking has the highest priority, overrides all other states
			// No horizontal movement while ducking
			state = MarioState::Ducking;
			if (velocity.x > 0) velocity.x -= DEC_SKID * dt; // Decelerate to a stop if ducking while moving right
			else if (velocity.x < 0) velocity.x += DEC_SKID * dt; // Decelerate to a stop if ducking while moving left
		}
		// GROUND PHYSICS
		if (abs(velocity.x) < MIN_WALK) {
			// Kickstart acceleration
			velocity.x = 0;
			if (input->IsKeyDown('A') && state != MarioState::Ducking) velocity.x -= MIN_WALK;
			if (input->IsKeyDown('D') && state != MarioState::Ducking) velocity.x += MIN_WALK;
		}
		else {
			// Moving: Handle Acceleration & Braking
			if (velocity.x > 0) { // Currently moving Right
				if (input->IsKeyDown('D')) {
					velocity.x += (input->IsKeyDown(VK_SHIFT) ? ACC_RUN : ACC_WALK) * dt;
				}
				else if (input->IsKeyDown('A')) {
					//Skidding
					velocity.x -= DEC_SKID * dt;
				}
				else {
					// No input, apply friction
					velocity.x -= DEC_REL * dt;
					if (velocity.x < 0) velocity.x = 0;
				}
			}
			else if (velocity.x < 0) { // Currently moving Left
				if (input->IsKeyDown('A')) {
					velocity.x -= (input->IsKeyDown(VK_SHIFT) ? ACC_RUN : ACC_WALK) * dt;
				}
				else if (input->IsKeyDown('D')) {
					// Skidding
					velocity.x += DEC_SKID * dt;
				}
				else {
					// No input, apply friction
					velocity.x += DEC_REL * dt;
					if (velocity.x > 0) velocity.x = 0;
				}
			}
		}
	}
	else
	{
		// AIR PHYSICS
		// Preserve momentum, only change if input is detected
		if (input->IsKeyDown('A')) {
			velocity.x -= (abs(velocity.x) > MAX_WALK ? ACC_RUN : ACC_WALK) * dt;
		}
		else if (input->IsKeyDown('D')) {
			velocity.x += (abs(velocity.x) > MAX_WALK ? ACC_RUN : ACC_WALK) * dt;
		}
	}

	// INITIATE JUMP
	if (input->IsKeyPressed('W') && isGrounded && state != MarioState::Ducking)
	{
		// jumped
		AudioManager::GetInstance()->PlaySFX(MARIO_JUMP_SMALL);

		if (abs(velocity.x) < 16.0f) {
			//Idle Jump
			velocity.y = -240.0f;
			fallAcc = STOP_FALL; // Heavy gravity
		}
		else if (abs(velocity.x) < 40.0f) {
			// Walk Jump
			velocity.y = -240.0f;
			fallAcc = WALK_FALL; // Very heavy gravity
		}
		else {
			//Run jump
			velocity.y = -300.0f; // Higher bounce due to momentum
			fallAcc = RUN_FALL;  // Extremely heavy gravity
		}
		isGrounded = false; // Lift off the ground
	}

	// VARIABLE JUMP
	if (!isGrounded && velocity.y < 0 && input->IsKeyDown('W'))
	{
		// Reduce gravity if the player is holding the Jump key
		if (fallAcc == STOP_FALL) velocity.y -= (STOP_FALL - STOP_FALL_A) * dt;
		if (fallAcc == WALK_FALL) velocity.y -= (WALK_FALL - WALK_FALL_A) * dt;
		if (fallAcc == RUN_FALL)  velocity.y -= (RUN_FALL - RUN_FALL_A) * dt;
	}

	auto fireBallCount = GetFireBallCount(coObjects);
	if (input->IsKeyPressed(VK_CONTROL)
		&& power == MarioPower::Fire
		&& state != MarioState::Ducking
		&& fireBallCount < MAX_FIREBALL_COUNT
		&& fireCooldownTimer.IsFinished()
		)
	{
		float offsetX = isFacingRight ? 16.0f : -16.0f; // Spawn fireball slightly in front of Mario
		float offsetY = 8.0f; // Spawn fireball slightly above Mario's center
		Fireball* f = new Fireball(position.x + offsetX, position.y + offsetY, isFacingRight);
		ctx->addObject(f);
		AudioManager::GetInstance()->PlaySFX(FIREBALL);
		fireCooldownTimer.Start();
	}

	// APPLY GRAVITY
	velocity.y += fallAcc * dt;


	// Y-axis clamping
	velocity.y = min(velocity.y, MAX_FALL);
	velocity.y = max(velocity.y, -MAX_FALL);

	// Absolute X-axis clamping
	velocity.x = min(velocity.x, MAX_RUN);
	velocity.x = max(velocity.x, -MAX_RUN);

	// Clamp back to Walk speed if Shift is released
	if (isGrounded) {
		if (velocity.x > MAX_WALK && !input->IsKeyDown(VK_SHIFT)) velocity.x = MAX_WALK;
		if (velocity.x < -MAX_WALK && !input->IsKeyDown(VK_SHIFT)) velocity.x = -MAX_WALK;
	}


	// UPDATE STATE & FACING DIRECTION

	// Update facing direction based on player input and only apply if grounded to prevent mid-air direction change
	if (input->IsKeyDown('A') && !input->IsKeyDown('D') && isGrounded && state != MarioState::Ducking) {
		isFacingRight = false;
	}
	else if (input->IsKeyDown('D') && !input->IsKeyDown('A') && isGrounded && state != MarioState::Ducking) {
		isFacingRight = true;
	}

	// Route Animation State
	if (!isGrounded) {
		state = MarioState::Jumping;
	}
	else {
		if ((power == MarioPower::Big || power == MarioPower::Fire) && input->IsKeyDown('S')) {
			state = MarioState::Ducking;
		}
		else if (abs(velocity.x) > MAX_WALK) {
			state = MarioState::Running;
		}
		else if (abs(velocity.x) >= MIN_WALK) {
			// Detect Skidding: Moving right but pressing left (or vice versa)
			if ((velocity.x > 0 && input->IsKeyDown('A')) || (velocity.x < 0 && input->IsKeyDown('D'))) {
				state = MarioState::Skidding;
			}
			else {
				state = MarioState::Walking;
			}
		}
		else {
			state = MarioState::Idle;
		}
	}
	fireCooldownTimer.ProcessTimer(dt);
	if (!fireCooldownTimer.IsFinished()) {
		state = MarioState::Firing;
	}
	Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);
}

void Mario::Render()
{
	auto g = Game::GetInstance();
	float renderX, renderY;
	g->GetCamera()
		->WorldToScreen(position.x, position.y, renderX, renderY);
	/*g->DrawDebugRectWithCamera(GetBoundingBox(), D3DXCOLOR(1, 0, 0, 1));*/

	if (power == MarioPower::Normal)
	{
		switch (state)
		{
		case MarioState::Dying:
			Animations::GetInstance()->Get(MARIO_DEATH_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
			break;
		case MarioState::Growing:
			Animations::GetInstance()->Get(MARIO_GROW_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
			break;
		case MarioState::Walking:
		case MarioState::Running:
			Animations::GetInstance()->Get(MARIO_RUN_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
			break;
		case MarioState::Skidding:
			Animations::GetInstance()->Get(MARIO_SKID_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
			break;
		case MarioState::Idle:
			Animations::GetInstance()->Get(MARIO_IDLE_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
			break;
		case MarioState::Jumping:
			Animations::GetInstance()->Get(MARIO_JUMP_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
			break;
		case MarioState::Firing:
			Animations::GetInstance()->Get(MARIO_IDLE_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
			break;
		default:
			DebugOut(L"[Error] No handling for state: %d\n", state);
		}
	}
	else if (power == MarioPower::Big)
	{
		switch (state)
		{
		case MarioState::Walking:
		case MarioState::Running:
			Animations::GetInstance()->Get(MARIO_BIG_RUN_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
			break;
		case MarioState::Skidding:
			Animations::GetInstance()->Get(MARIO_BIG_SKID_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
			break;
		case MarioState::Idle:
			Animations::GetInstance()->Get(MARIO_BIG_IDLE_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
			break;
		case MarioState::Jumping:
			Animations::GetInstance()->Get(MARIO_BIG_JUMP_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
			break;
		case MarioState::Ducking:
			Animations::GetInstance()->Get(MARIO_BIG_DUCK_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
			break;
		default:
			DebugOut(L"[Error] No handling for state: %d\n", state);
		}
	}
	else if (power == MarioPower::Fire)
	{
		switch (state)
		{
		case MarioState::Walking:
		case MarioState::Running:
			Animations::GetInstance()->Get(MARIO_FIRE_RUN_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
			break;
		case MarioState::Skidding:
			Animations::GetInstance()->Get(MARIO_FIRE_SKID_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
			break;
		case MarioState::Idle:
			Animations::GetInstance()->Get(MARIO_FIRE_IDLE_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
			break;
		case MarioState::Jumping:
			Animations::GetInstance()->Get(MARIO_FIRE_JUMP_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
			break;
		case MarioState::Ducking:
			Animations::GetInstance()->Get(MARIO_FIRE_DUCK_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
			break;
		case MarioState::Firing:
			Animations::GetInstance()->Get(MARIO_FIRE_FIRE_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
			break;
		default:
			DebugOut(L"[Error] No handling for state: %d\n", state);
		}
	}
}

Rect Mario::GetBoundingBox()
{
	RectF r;
	if (power == MarioPower::Normal)
	{
		r.top = position.y + 3;
		r.left = position.x + 1;
		r.bottom = position.y + 16;
		r.right = position.x + 14;
	}
	else if (power == MarioPower::Big || power == MarioPower::Fire)
	{
		r.top = position.y + 5;
		r.left = position.x + 2;
		r.bottom = position.y + 32;
		r.right = position.x + 14;
	}
	return r;
}

void Mario::OnNoCollision(float dt)
{
	position += velocity * dt;
	isGrounded = false;
}

void Mario::OnCollisionWith(CollisionEvent* e)
{
	//DebugOut(L"Normal %d, %d\n", e->normalizedDir.x, e->normalizedDir.y);

	if (e->IsTileCollision())
	{
		// resolve tile collision
		if (e->otherTile->IsBlocking()
			&& e->normalizedDir.y == -1
			&& e->normalizedDir.x == 0
			)
		{
			isGrounded = true;
		}
	}
	else if (e->IsObjectCollision())
	{
		// resolve object collision
		const auto goomba = dynamic_cast<Goomba*>(e->otherObject);

		if (goomba != nullptr)
		{
			if (goomba->GetState() == GoombaState::Dead)
				return;

			if (e->normalizedDir.y == -1)
			{
				// jump on head

				velocity.y = -240.0f;
				state = MarioState::Jumping;


				goomba->SetState(GoombaState::Dead);

				goombaKilled++;
				AudioManager::GetInstance()->PlaySFX(GOOMBA_STOMP);

			}
			else
			{
				if (power != MarioPower::Normal)
				{
					power = MarioPower::Normal;
					state = MarioState::Idle;
					return;
				}
				// got kill by goomba, bad
				velocity.y = -250.0f;
				velocity.x = 0;
				isCollidable = false;
				state = MarioState::Dying;
				AudioManager::GetInstance()->StopAll();
				AudioManager::GetInstance()->PlaySFX(MARIO_DIE);
			}
			return;
		}

		const auto portal = dynamic_cast<NextLevelPortal*>(e->otherObject);
		if (portal != nullptr)
		{
			portal->RequestNextLevel();
			return;
		}
		const auto questionBlock = dynamic_cast<QuestionBlock*>(e->otherObject);
		if (questionBlock != nullptr) {
			if (e->normalizedDir.y == -1)
			{
				isGrounded = true;
				return;
			}

			if (e->normalizedDir.y == 1)
			{

				if (!questionBlock->HaveDrop())
				{
					if (power == MarioPower::Big || power == MarioPower::Fire)
					{
						questionBlock->SetState(QuestionBlockState::Break);
					}
				}
				else
				{
					questionBlock->SetState(QuestionBlockState::Opened);
				}
			}
		}

		const auto coin = dynamic_cast<Coin*>(e->otherObject);
		if (coin != nullptr)
		{
			coin->SetState(CoinState::Collected);
			coinCollected++;
			AudioManager::GetInstance()->PlaySFX(MARIO_COLLECT_COIN);

		}

		const auto mushroom = dynamic_cast<Mushroom*>(e->otherObject);
		if (mushroom != nullptr)
		{
			mushroom->SetState(CollectableItemState::Collected);
			AudioManager::GetInstance()->PlaySFX(MARIO_POWERUP);

			if (power == MarioPower::Normal)
			{
				state = MarioState::Growing;
				transformTimer = TRANSFORM_TIME;
				// add some pushback so player won't fall off the ground
				position.y -= 17;
			}
		}

		const auto flower = dynamic_cast<Flower*>(e->otherObject);
		if (flower != nullptr)
		{
			flower->SetState(CollectableItemState::Collected);
			AudioManager::GetInstance()->PlaySFX(MARIO_POWERUP);

			if (power == MarioPower::Big)
			{
				power = MarioPower::Fire;
			}
		}
	}
}
