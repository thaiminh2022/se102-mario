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
#include "NextLevelPortal.h"

constexpr float GRAVITY = 900.0f;


Mario::Mario(int startX, int startY) : GameObject(static_cast<float>(startX), static_cast<float>(startY))

{
	auto marioTex = Textures::GetInstance()->Get(MARIO_TEX_ID);
	auto anims = Animations::GetInstance();
	auto sprites = Sprites::GetInstance();

	// sprites
	sprites->Add(MARIO_RUN_SPRITE_1, 20, 0, 35, 15, marioTex);
	sprites->Add(MARIO_RUN_SPRITE_2, 38, 0, 53, 15, marioTex);
	sprites->Add(MARIO_RUN_SPRITE_3, 56, 0, 71, 15, marioTex);

	sprites->Add(MARIO_TURN_SPRITE_1, 76, 0, 91, 15, marioTex);

	sprites->Add(MARIO_JUMP_SPRITE_1, 96, 0, 111, 15, marioTex);

	sprites->Add(MARIO_IDLE_SPRITE_1, 0, 0, 15, 15, marioTex);

	sprites->Add(MARIO_DEATH_SPRITE_1, 116, 0, 131, 15, marioTex);

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

	// turn anim
	anim = new Animation(100);
	anim->Add(MARIO_TURN_SPRITE_1);
	anims->Add(MARIO_TURN_ANIM_ID, anim);

	// jump anim
	anim = new Animation(100);
	anim->Add(MARIO_JUMP_SPRITE_1);
	anims->Add(MARIO_JUMP_ANIM_ID, anim);

	// death anim
	anim = new Animation(100);
	anim->Add(MARIO_DEATH_SPRITE_1);
	anims->Add(MARIO_DEATH_ANIM_ID, anim);

	isGrounded = false;
	isCollidable = true;
	isFacingRight = true;
	velocity.x = 0.0f;
	velocity.y = 0.0f;
	state = MarioState::Idle;


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

	auto input = InputManager::GetInstance();
	if (isGrounded)
	{
		// GROUND PHYSICS
		if (abs(velocity.x) < MIN_WALK) {
			// Kickstart acceleration
			velocity.x = 0;
			if (input->IsKeyDown('A') && !input->IsKeyDown('S')) velocity.x -= MIN_WALK;
			if (input->IsKeyDown('D') && !input->IsKeyDown('S')) velocity.x += MIN_WALK;
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
	if (input->IsKeyPressed('W') && isGrounded)
	{
		if (abs(velocity.x) < 16.0f) {
			//Idle Jump
			velocity.y = -280.0f;
			fallAcc = STOP_FALL; // Heavy gravity
		}
		else if (abs(velocity.x) < 40.0f) {
			// Walk Jump
			velocity.y = -280.0f;
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

	// APPLY GRAVITY
	velocity.y += fallAcc * dt;


	// Y-axis clamping (Only limit fall speed, not jump force)
	velocity.y = min(velocity.y, MAX_FALL);

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
	if (input->IsKeyDown('A') && !input->IsKeyDown('D') && isGrounded) {
		isFacingRight = false;
	}
	else if (input->IsKeyDown('D') && !input->IsKeyDown('A') && isGrounded) {
		isFacingRight = true;
	}

	// Route Animation State
	if (!isGrounded) {
		state = MarioState::Jumping;
	}
	else {
		if (input->IsKeyDown('S')) {
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

	Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);
}

void Mario::Render()
{
	float renderX, renderY;
	Game::GetInstance()
		->GetCamera()
		->WorldToScreen(position.x, position.y, renderX, renderY);

	switch (state)
	{
	case MarioState::Dying:
		Animations::GetInstance()->Get(MARIO_DEATH_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
		break;
	case MarioState::Walking:
	case MarioState::Running:
	//case MarioState::Ducking:
		Animations::GetInstance()->Get(MARIO_RUN_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
		break;
	case MarioState::Skidding:
		Animations::GetInstance()->Get(MARIO_TURN_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
		break;
	case MarioState::Dead:
	case MarioState::Idle:
		Animations::GetInstance()->Get(MARIO_IDLE_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
		break;
	case MarioState::Jumping:
		Animations::GetInstance()->Get(MARIO_JUMP_ANIM_ID)->Render(round(renderX), round(renderY), !isFacingRight, false);
		break;
	default:
		DebugOut(L"[Error] No handling for state: %d\n", state);
	}
	

}

Rect Mario::GetBoundingBox()
{
	RectF r;
	r.top = position.y;
	r.left = position.x;
	r.bottom = position.y + 16;
	r.right = position.x + 16;
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
				
				AudioManager::GetInstance()->PlaySFX(GOOMBA_STOMP);

			}else
			{
				// got kill by goomba, bad
				velocity.y = -250.0f;
				velocity.x = 0;
				isCollidable = false;
				state = MarioState::Dead;
				AudioManager::GetInstance()->StopAll();
				AudioManager::GetInstance()->PlaySFX(MARIO_DIE);
			}
		}

		const auto portal = dynamic_cast<NextLevelPortal*>(e->otherObject);
		if (portal != nullptr)
		{
			portal->RequestNextLevel();
		}
	}
}
