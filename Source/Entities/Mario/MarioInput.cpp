#include "Animation.h"
#include "AssetIDs.h"
#include "AudioManager.h"
#include "Fireball.h"
#include "InputManager.h"
#include "Mario.h"
#include "MarioBreathBubble.h"



void Mario::WhileGrounded(float dt)
{
	const auto input = InputManager::GetInstance();
	if (input->IsKeyDown('S') && (power == MarioPower::Big || power == MarioPower::Fire || power == MarioPower::StarmanBig || power == MarioPower::Raccoon))
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
				if (!skidTimer.IsTicking() && power == MarioPower::Raccoon)
				{
					AudioManager::GetInstance()->PlaySFX(TWIRL);
					skidTimer.Start();
				}
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
				if (!skidTimer.IsTicking() && power == MarioPower:: Raccoon)
				{
					AudioManager::GetInstance()->PlaySFX(TWIRL);
					skidTimer.Start();
				}
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
		if (power == MarioPower::Raccoon && raccoonSuit) {
			velocity.x -= ACC_RUN * dt; //faster acceleration in air for raccoon mario with raccoon suit

		}
		else {
			// normal jumping
			velocity.x -= (abs(velocity.x) > MAX_WALK ? ACC_RUN : ACC_WALK) * dt;
		}
	}
	else if (input->IsKeyDown('D'))
	{

		if (power == MarioPower::Raccoon && raccoonSuit) {
			velocity.x += ACC_RUN * dt; //faster acceleration in air for raccoon mario with raccoon suit
		}
		else {
			// normal jumping
			velocity.x += (abs(velocity.x) > MAX_WALK ? ACC_RUN : ACC_WALK) * dt;
		}
	}
	//facing directon when flying will be handled in UpdateFacingDirection()
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

void Mario::HandleRaccoonSuit(float dt)
{
	if (raccoonSuit == nullptr || isInWater)
		return;

	const auto input = InputManager::GetInstance();
	const bool movingWithInput =
		(velocity.x > 0.0f && input->IsKeyDown('D')) ||
		(velocity.x < 0.0f && input->IsKeyDown('A'));
	const bool canCharge =
		isGrounded &&
		state != MarioState::Ducking &&
		input->IsKeyDown(VK_SHIFT) &&
		movingWithInput &&
		abs(velocity.x) >= PMETER_MIN_RUN_SPEED;

	if (isGrounded)
	{
		raccoonSuit->UpdateMeter(dt, canCharge);
		return;
	}

	// Process SFX timer globally so it ticks properly independent of key presses
	if (tailSFXTimer.IsTicking())
	{
		tailSFXTimer.ProcessTimer(dt);
	}
	
	// FLYING STATE (P-Meter is full)
	if (raccoonSuit->ReadyToFly())
	{
		state = MarioState::Flying;

		// Process flight timer continuously regardless of input
		if (!raccoonFlyingTimer.IsTicking())
		{
			raccoonFlyingTimer = Timer(RACCOON_FLYING_TIME_LIMIT);
			raccoonFlyingTimer.Start();
		}
		else
		{
			raccoonFlyingTimer.ProcessTimer(dt);

			// Check if flight time is over
			if (raccoonFlyingTimer.IsFinished())
			{
				raccoonFlyingTimer.SetIdle();
				raccoonSuit->SetPMeter(0); // Reset P-Meter, falling begins
			}
		}

		// Handle flapping input (Impulse logic)
		if (input->IsKeyPressed('W'))
		{
			// Apply a strong upward impulse to lift Mario
			velocity.y = RACCOON_FLY_IMPULSE;
			// Handle SFX based on remaining flight time
			if (!tailSFXTimer.IsTicking() || tailSFXTimer.IsFinished())
			{
				float timeRatio = raccoonFlyingTimer.GetTimeLeft() / raccoonFlyingTimer.GetStartTime();

				if (timeRatio <= LOW_FLYING_TIME_PERCENT) {
					AudioManager::GetInstance()->PlaySFX(TAIL);
				}

				tailSFXTimer = Timer(TAIL_SFX_INTERVAL);
				tailSFXTimer.Start();
			}
			if (!raccoonFloatingTimer.IsTicking())
			{
				fallAcc = RACCOON_FLYING_GRAVITY; // Lower gravity while flying
				raccoonFloatingTimer = Timer(RACCOON_FLOATING_TIME);
				raccoonFloatingTimer.Start();
			}
		}
	}
	// FLOATING STATE (Falling and P-Meter is NOT full)
	else if (velocity.y > 0.0f)
	{
		// Pressing 'W' repeatedly stalls the fall but won't lift Mario up
		if (input->IsKeyPressed('W'))
		{
			// Setting velocity to a small positive number limits downward momentum instantly
			velocity.y = RACCOON_WAG_VELOCITY;

			if (!tailSFXTimer.IsTicking() || tailSFXTimer.IsFinished())
			{
				AudioManager::GetInstance()->PlaySFX(TAIL);
				tailSFXTimer = Timer(TAIL_SFX_INTERVAL);
				tailSFXTimer.Start();
			}

			if (!raccoonFloatingTimer.IsTicking())
			{
				fallAcc = RACCOON_FLYING_GRAVITY; // Lower gravity while flying
				raccoonFloatingTimer = Timer(RACCOON_FLOATING_TIME);
				raccoonFloatingTimer.Start();
			}
		}
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
	if (input->IsKeyDown('A') && !input->IsKeyDown('D') && (isGrounded || isInWater || (power == MarioPower::Raccoon && raccoonSuit)) && state != MarioState::Ducking)
	{
		isFacingRight = false;
	}
	else if (input->IsKeyDown('D') && !input->IsKeyDown('A') && (isGrounded || isInWater || (power == MarioPower::Raccoon && raccoonSuit)) && state != MarioState::Ducking)
	{
		isFacingRight = true;
	}
}
void Mario::ApplyGravityAndClamp(float dt)
{
	const auto input = InputManager::GetInstance();

	// Normal gravity always applies. Wagging resets this built-up velocity.
	velocity.y += fallAcc * dt;

	

	if (isInWater)
	{
		velocity.x = min(velocity.x, MAX_SWIM);
		velocity.x = max(velocity.x, -MAX_SWIM);
		velocity.y = min(velocity.y, WATER_MAX_FALL);
		velocity.y = max(velocity.y, SWIM_UP_SPEED);
	}
	else
	{
		const bool hasRaccoonSuit = (raccoonSuit != nullptr);

		float currentMaxSpeed = MAX_RUN;
		if (hasRaccoonSuit && raccoonSuit->ReadyToFly())
		{
			currentMaxSpeed = MAX_RACCOON_RUN; // Higher max speed when running with fully charged P-Meter raccoon suit
		}

		velocity.x = min(velocity.x, currentMaxSpeed);
		velocity.x = max(velocity.x, -currentMaxSpeed);

		const bool isFlying = hasRaccoonSuit && raccoonSuit->ReadyToFly();

		// If no button is pressed, Mario falls at normal MAX_FALL speed.
		// Slow floating is achieved dynamically by assigning RACCOON_WAG_VELOCITY on 'W' press.
		velocity.y = min(velocity.y, MAX_FALL);

		if (isFlying) {
			velocity.y = max(velocity.y, RACCOON_MAX_RISE);
		}
		else {
			velocity.y = max(velocity.y, -MAX_FALL);
		}
	}

	if (isGrounded)
	{
		if (velocity.x > MAX_WALK && !input->IsKeyDown(VK_SHIFT)) velocity.x = MAX_WALK;
		if (velocity.x < -MAX_WALK && !input->IsKeyDown(VK_SHIFT)) velocity.x = -MAX_WALK;
	}
	DebugOutTitle(L"Velocity: %.2f, %.2f\n", velocity.x, velocity.y);
}
