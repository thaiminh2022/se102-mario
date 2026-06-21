#include "Animations.h"
#include "AssetIDs.h"
#include "Debug.h"
#include "Game.h"
#include "InputManager.h"
#include "Mario.h"

void Mario::Render()
{
	if (!isRendering)
		return;

	if (isInvincible)
	{
		if ((GetTickCount() / 100) % 2 == 0)
		{
			return; // Skip this frame to create a blinking effect
		}
	}

	auto g = Game::GetInstance();
	float renderX, renderY;
	g->GetCamera()
		->WorldToScreen(position.x, position.y, renderX, renderY);

	auto animId = GetMarioAnimId();

	Animations::GetInstance()
		->Get(animId)
		->Render(round(renderX), round(renderY), !isFacingRight, false);
}

int Mario::GetMarioAnimId() const
{
	if (power == MarioPower::Normal)
	{
		switch (state)
		{
		case MarioState::Dying:
			return MARIO_DEATH_ANIM_ID;
		case MarioState::Growing:
			return MARIO_GROWBIG_ANIM_ID;
		case MarioState::Walking:
		case MarioState::WalkingToCastle:
		case MarioState::EnteringPipe:
		case MarioState::ForceMoving:
			return MARIO_WALK_ANIM_ID;
		case MarioState::Running:
			return MARIO_RUN_ANIM_ID;
		case MarioState::Skidding:
			return MARIO_SKID_ANIM_ID;
		case MarioState::Idle:
			return MARIO_IDLE_ANIM_ID;
		case MarioState::Jumping:
			return  isInWater ? MARIO_SWIM_ANIM_ID : MARIO_JUMP_ANIM_ID;
		case MarioState::Firing:
			return MARIO_IDLE_ANIM_ID;
		case MarioState::PullingFlag:
			return MARIO_FLAG_PULL_ANIM_ID;
		case MarioState::StopToWaitBowser:
			return MARIO_JUMP_ANIM_ID;
		default:
			DebugOut(L"[WARNING] No handling for state: %d\n", state);
		}
	}
	else if (power == MarioPower::Big)
	{
		switch (state)
		{
		case MarioState::Walking:
		case MarioState::WalkingToCastle:
		case MarioState::EnteringPipe:
		case MarioState::ForceMoving:
			return MARIO_BIG_WALK_ANIM_ID;
		case MarioState::Running:
			return MARIO_BIG_RUN_ANIM_ID;
		case MarioState::Skidding:
			return MARIO_BIG_SKID_ANIM_ID;
		case MarioState::Idle:
			return MARIO_BIG_IDLE_ANIM_ID;
		case MarioState::Jumping:
			return isInWater ? MARIO_BIG_SWIM_ANIM_ID : MARIO_BIG_JUMP_ANIM_ID;
		case MarioState::Ducking:
			return MARIO_BIG_DUCK_ANIM_ID;
		case MarioState::Shrinking:
			return MARIO_SHRINK_ANIM_ID;
		case MarioState::PullingFlag:
			return MARIO_BIG_FLAG_PULL_ANIM_ID;
		default:
			DebugOut(L"[WARNING] No handling for state: %d\n", state);
		}
	}
	else if (power == MarioPower::Fire)
	{
		switch (state)
		{
		case MarioState::Walking:
		case MarioState::WalkingToCastle:
		case MarioState::EnteringPipe:
		case MarioState::ForceMoving:
			return MARIO_FIRE_WALK_ANIM_ID;
		case MarioState::Running:
			return MARIO_FIRE_RUN_ANIM_ID;
		case MarioState::Skidding:
			return MARIO_FIRE_SKID_ANIM_ID;
		case MarioState::Idle:
			return MARIO_FIRE_IDLE_ANIM_ID;
		case MarioState::Jumping:
			return isInWater ? MARIO_FIRE_SWIM_ANIM_ID : MARIO_FIRE_JUMP_ANIM_ID;
		case MarioState::Ducking:
			return MARIO_FIRE_DUCK_ANIM_ID;
		case MarioState::Firing:
			return MARIO_FIRE_FIRE_ANIM_ID;
		case MarioState::Shrinking:
			return MARIO_SHRINK_ANIM_ID;
		case MarioState::PullingFlag:
			return MARIO_FIRE_FLAG_PULL_ANIM_ID;
		default:
			DebugOut(L"[Error] No handling for state: %d\n", state);
		}
	}
	else if (power == MarioPower::StarmanBig) {
		switch (state)
		{
		case MarioState::Walking:
		case MarioState::WalkingToCastle:
		case MarioState::EnteringPipe:
		case MarioState::ForceMoving:
			return STARMAN_BIG1_WALK_ANIM_ID;
		case MarioState::Running:
			return STARMAN_BIG1_RUN_ANIM_ID;
		case MarioState::Skidding:
			return STARMAN_BIG1_SKID_ANIM_ID;
		case MarioState::Idle:
			return STARMAN_BIG1_IDLE_ANIM_ID;
		case MarioState::Jumping:
			return isInWater ? STARMAN_BIG1_SWIM_ANIM_ID : STARMAN_BIG1_JUMP_ANIM_ID;
		case MarioState::Ducking:
			return STARMAN_BIG1_DUCK_ANIM_ID;
		case MarioState::PullingFlag:
			return STARMAN_BIG1_FLAG_PULL_ANIM_ID;
		default:
			DebugOut(L"[Error] No handling for state: %d\n", state);
		}
	}
	else if (power == MarioPower::StarmanSmall) {
		switch (state)
		{
		case MarioState::Walking:
		case MarioState::WalkingToCastle:
		case MarioState::EnteringPipe:
		case MarioState::ForceMoving:
			return STARMAN_SMALL1_WALK_ANIM_ID;
		case MarioState::Running:
			return STARMAN_SMALL1_RUN_ANIM_ID;
		case MarioState::Skidding:
			return STARMAN_SMALL1_SKID_ANIM_ID;
		case MarioState::Idle:
			return STARMAN_SMALL1_IDLE_ANIM_ID;
		case MarioState::Jumping:
			return isInWater ? STARMAN_SMALL1_SWIM_ANIM_ID :  STARMAN_SMALL1_JUMP_ANIM_ID;
		case MarioState::PullingFlag:
			return STARMAN_SMALL1_FLAG_PULL_ANIM_ID;
		default:
			DebugOut(L"[Error] No handling for state: %d\n", state);
		}
	}
	else if (power == MarioPower::Raccoon)
	{
		switch (state)
		{
			case MarioState::Walking:
			case MarioState::WalkingToCastle:
			case MarioState::EnteringPipe:
			case MarioState::ForceMoving:
				return RACCOON_WALK_ANIM_ID;
			case MarioState::Running:
				return (raccoonSuit!= nullptr && raccoonSuit->ReadyToFly()) ? RACCOON_PREFLY_ANIM_ID : RACCOON_RUN_ANIM_ID;
			case MarioState::Skidding:
				return RACCOON_SKID_ANIM_ID;
			case MarioState::Idle:
				return RACCOON_IDLE_ANIM_ID;
			case MarioState::Jumping:
				return isInWater ? MARIO_BIG_SWIM_ANIM_ID : RACCOON_JUMP_ANIM_ID;
			case MarioState::Ducking:
				return RACCOON_DUCK_ANIM_ID;
			case MarioState::Flying:
				return RACCOON_FLY_ANIM_ID;
			case MarioState::PullingFlag:
				return RACCOON_FLAGPULL_ANIM_ID;
			default:
				DebugOut(L"[Error] No handling for state: %d\n", state);
		}
	}
	return MARIO_IDLE_ANIM_ID;
}


void Mario::RouteAnimationState()
{
	const InputManager* input = InputManager::GetInstance();
	if (!isGrounded)
	{
		lastState = state;
		if (raccoonSuit != nullptr && raccoonSuit->ReadyToFly() )
		{
			state = MarioState::Flying;
		}
		else
			state = MarioState::Jumping;
	}
	else
	{
		if ((power == MarioPower::Big || power == MarioPower::Fire || power == MarioPower::StarmanBig || power == MarioPower::Raccoon) && input->IsKeyDown('S'))
		{
			lastState = state;
			state = MarioState::Ducking;
		}
		else if (abs(velocity.x) > MAX_WALK)
		{
			lastState = state;
			state = MarioState::Running;
		}
		else if (abs(velocity.x) >= MIN_WALK)
		{
			// Detect Skidding: Moving right but pressing left (or vice versa)
			if ((velocity.x > 0 && input->IsKeyDown('A')) || (velocity.x < 0 && input->IsKeyDown('D')))
			{
				lastState = state;
				state = MarioState::Skidding;
			}
			else
			{
				lastState = state;
				state = MarioState::Walking;
			}
		}
		else
		{
			lastState = state;
			state = MarioState::Idle;
		}
	}
}
