#include "Animation.h"
#include "AssetIDs.h"
#include "AudioManager.h"
#include "Fireball.h"
#include "InputManager.h"
#include "Mario.h"
#include "MarioBreathBubble.h"

const float SWIM_UP_SPEED = -150.0f;   // Upward impulse/speed when pressing swim
const float WATER_GRAVITY = 180.0f;    // Slow underwater downward acceleration
const float WATER_MAX_FALL = 90.0f;    // Slow sinking cap
const float MAX_SWIM = 100.0f;         // horizontal cap

void Mario::WhileGrounded(float dt)
{
	const auto input = InputManager::GetInstance();
	if (input->IsKeyDown('S') && (power == MarioPower::Big || power == MarioPower::Fire))
	{
		state = MarioState::Ducking;
		if (velocity.x > 0) velocity.x -= DEC_SKID * dt; // Decelerate to a stop if ducking while moving right
		else if (velocity.x < 0) velocity.x += DEC_SKID * dt; // Decelerate to a stop if ducking while moving left
	}
	// GROUND PHYSICS
	if (abs(velocity.x) < MIN_WALK)
	{
		// Kickstart acceleration
		velocity.x = 0;
		if (input->IsKeyDown('A') && state != MarioState::Ducking) velocity.x -= MIN_WALK;
		if (input->IsKeyDown('D') && state != MarioState::Ducking) velocity.x += MIN_WALK;
	}
	else
	{
		// Moving: Handle Acceleration & Braking
		if (velocity.x > 0)
		{
			// Currently moving Right
			if (input->IsKeyDown('D'))
			{
				velocity.x += (input->IsKeyDown(VK_SHIFT) ? ACC_RUN : ACC_WALK) * dt;
			}
			else if (input->IsKeyDown('A'))
			{
				//Skidding
				velocity.x -= DEC_SKID * dt;
			}
			else
			{
				// No input, apply friction
				velocity.x -= DEC_REL * dt;
				velocity.x = std::max<float>(velocity.x, 0);
			}
		}
		else if (velocity.x < 0)
		{
			// Currently moving Left
			if (input->IsKeyDown('A'))
			{
				velocity.x -= (input->IsKeyDown(VK_SHIFT) ? ACC_RUN : ACC_WALK) * dt;
			}
			else if (input->IsKeyDown('D'))
			{
				// Skidding
				velocity.x += DEC_SKID * dt;
			}
			else
			{
				// No input, apply friction
				velocity.x += DEC_REL * dt;
				velocity.x = std::min<float>(velocity.x, 0);
			}
		}
	}
}

void Mario::WhileOnAir(float dt)
{

	const auto input = InputManager::GetInstance();

	// AIR PHYSICS
	// Preserve momentum, only change if input is detected
	if (input->IsKeyDown('A'))
	{
		velocity.x -= (abs(velocity.x) > MAX_WALK ? ACC_RUN : ACC_WALK) * dt;
	}
	else if (input->IsKeyDown('D'))
	{
		velocity.x += (abs(velocity.x) > MAX_WALK ? ACC_RUN : ACC_WALK) * dt;
	}
	

}

void Mario::HandleSwim(float dt, const SceneContext* ctx)
{
	if (!isInWater)
		return;

	breathingTimer.ProcessTimer(dt);
	if (breathingTimer.IsFinished())
	{
		const auto bubble = new MarioBreathBubble(position + Vector2(8, 0));
		ctx->addObject(bubble);
		breathingTimer.Start();
	}

	const auto input = InputManager::GetInstance();
	fallAcc = WATER_GRAVITY;
	if (input->IsKeyDownThisFrame('W'))
	{
		if (!isGrounded)
		{
			AudioManager::GetInstance()->PlaySFX(MARIO_JUMP_SMALL);
		}
		velocity.y = SWIM_UP_SPEED;
	}
}

void Mario::HandleJump(float dt)
{
	const auto input = InputManager::GetInstance();

	// INITIATE JUMP
	if (input->IsKeyPressed('W') && isGrounded && state != MarioState::Ducking)
	{
		// jumped
		AudioManager::GetInstance()->PlaySFX(MARIO_JUMP_SMALL);

		if (abs(velocity.x) < 16.0f)
		{
			//Idle Jump
			velocity.y = -240.0f;
			fallAcc = STOP_FALL; // Heavy gravity
		}
		else if (abs(velocity.x) < 40.0f)
		{
			// Walk Jump
			velocity.y = -240.0f;
			fallAcc = WALK_FALL; // Very heavy gravity
		}
		else
		{
			//Run jump
			velocity.y = -300.0f; // Higher bounce due to momentum
			fallAcc = RUN_FALL; // Extremely heavy gravity
		}

		// can keep jumping
		if (jetpack && jetpack->ReadyToFly())
			return;

		isGrounded = false; // Lift off the ground
	}

	// VARIABLE JUMP
	if (!isGrounded && velocity.y < 0 && input->IsKeyDown('W'))
	{
		// Reduce gravity if the player is holding the Jump key
		if (fallAcc == STOP_FALL) velocity.y -= (STOP_FALL - STOP_FALL_A) * dt;
		if (fallAcc == WALK_FALL) velocity.y -= (WALK_FALL - WALK_FALL_A) * dt;
		if (fallAcc == RUN_FALL) velocity.y -= (RUN_FALL - RUN_FALL_A) * dt;
	}
}

void Mario::HandleShootFireball(const float dt, const vector<GameObject*>& coObjects, const SceneContext* ctx)
{
	if (power == MarioPower::Fire)
	{
		auto fireBallCount = GetFireBallCount(coObjects);
		if (InputManager::GetInstance()->IsKeyPressed(VK_CONTROL)
			&& state != MarioState::Ducking
			&& fireBallCount < MAX_FIREBALL_COUNT
			&& fireCooldownTimer.IsFinished()
			)
		{
			float offsetX = isFacingRight ? 16.0f : -16.0f; // Spawn fireball slightly in front of Mario
			float offsetY = 8.0f; // Spawn fireball slightly above Mario's center
			auto f = new Fireball(position.x + offsetX, position.y + offsetY, isFacingRight);
			ctx->addObject(f);
			AudioManager::GetInstance()->PlaySFX(FIREBALL);
			fireCooldownTimer.Start();
		}
	}
	fireCooldownTimer.ProcessTimer(dt);
	if (!fireCooldownTimer.IsFinished())
	{
		state = MarioState::Firing;
	}
}

void Mario::UpdateFacingDirection()
{
	const auto input = InputManager::GetInstance();
	if (input->IsKeyDown('A') && !input->IsKeyDown('D') && (isGrounded || isInWater) && state != MarioState::Ducking)
	{
		isFacingRight = false;
	}
	else if (input->IsKeyDown('D') && !input->IsKeyDown('A') && (isGrounded || isInWater) && state != MarioState::Ducking)
	{
		isFacingRight = true;
	}
}
void Mario::ApplyGravityAndClamp(float dt)
{
	const auto input = InputManager::GetInstance();

	velocity.y += fallAcc * dt;

	
	if (isInWater)
	{
		velocity.x = min(velocity.x, MAX_SWIM);
		velocity.x = max(velocity.x, -MAX_SWIM);
		velocity.y = min(velocity.y, WATER_MAX_FALL);
		velocity.y = max(velocity.y, SWIM_UP_SPEED);
	}else
	{
		velocity.x = min(velocity.x, MAX_RUN);
		velocity.x = max(velocity.x, -MAX_RUN);
		velocity.y = min(velocity.y, MAX_FALL);
		velocity.y = max(velocity.y, -MAX_FALL);
	}


	if (isGrounded)
	{
		if (velocity.x > MAX_WALK && !input->IsKeyDown(VK_SHIFT)) velocity.x = MAX_WALK;
		if (velocity.x < -MAX_WALK && !input->IsKeyDown(VK_SHIFT)) velocity.x = -MAX_WALK;
	}
}
