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
		case MarioState::Running:
		case MarioState::WalkingToCastle:
		case MarioState::EnteringPipe:
			return MARIO_RUN_ANIM_ID;
		case MarioState::Skidding:
			return MARIO_SKID_ANIM_ID;
		case MarioState::Idle:
			return MARIO_IDLE_ANIM_ID;
		case MarioState::Jumping:
			return MARIO_JUMP_ANIM_ID;
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
		case MarioState::Running:
		case MarioState::WalkingToCastle:
		case MarioState::EnteringPipe:
			return MARIO_BIG_RUN_ANIM_ID;
		case MarioState::Skidding:

			return MARIO_BIG_SKID_ANIM_ID;
		case MarioState::Idle:
			return MARIO_BIG_IDLE_ANIM_ID;
		case MarioState::Jumping:
			return MARIO_BIG_JUMP_ANIM_ID;
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
		case MarioState::Running:
		case MarioState::WalkingToCastle:
		case MarioState::EnteringPipe:
			return MARIO_FIRE_RUN_ANIM_ID;
		case MarioState::Skidding:
			return MARIO_FIRE_SKID_ANIM_ID;
		case MarioState::Idle:
			return MARIO_FIRE_IDLE_ANIM_ID;
		case MarioState::Jumping:
			return MARIO_FIRE_JUMP_ANIM_ID;
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
		case MarioState::Running:
		case MarioState::WalkingToCastle:
		case MarioState::EnteringPipe:
			return STARMAN_BIG_RUN_ANIM_ID;
		case MarioState::Skidding:
			return STARMAN_BIG_SKID_ANIM_ID;
		case MarioState::Idle:
			return STARMAN_BIG_IDLE_ANIM_ID;
		case MarioState::Jumping:
			return STARMAN_BIG_JUMP_ANIM_ID;
		case MarioState::Ducking:
			return STARMAN_BIG_DUCK_ANIM_ID;
		case MarioState::PullingFlag:
			return STARMAN_BIG_FLAG_PULL_ANIM_ID;
		default:
			DebugOut(L"[Error] No handling for state: %d\n", state);
		}
	}
	else if (power == MarioPower::StarmanSmall) {
		switch (state)
		{
		case MarioState::Walking:
		case MarioState::Running:
		case MarioState::WalkingToCastle:
		case MarioState::EnteringPipe:
			return STARMAN_SMALL_RUN_ANIM_ID;
		case MarioState::Skidding:
			return STARMAN_SMALL_SKID_ANIM_ID;
		case MarioState::Idle:
			return STARMAN_SMALL_IDLE_ANIM_ID;
		case MarioState::Jumping:
			return STARMAN_SMALL_JUMP_ANIM_ID;
		case MarioState::PullingFlag:
			return STARMAN_SMALL_FLAG_PULL_ANIM_ID;
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
		state = MarioState::Jumping;
	}
	else
	{
		if ((power == MarioPower::Big || power == MarioPower::Fire) && input->IsKeyDown('S'))
		{
			state = MarioState::Ducking;
		}
		else if (abs(velocity.x) > MAX_WALK)
		{
			state = MarioState::Running;
		}
		else if (abs(velocity.x) >= MIN_WALK)
		{
			// Detect Skidding: Moving right but pressing left (or vice versa)
			if ((velocity.x > 0 && input->IsKeyDown('A')) || (velocity.x < 0 && input->IsKeyDown('D')))
			{
				state = MarioState::Skidding;
			}
			else
			{
				state = MarioState::Walking;
			}
		}
		else
		{
			state = MarioState::Idle;
		}
	}
}
