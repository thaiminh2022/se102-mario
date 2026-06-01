
#include "AssetIDs.h"
#include "AudioManager.h"
#include "Collision.h"
#include "Debug.h"
#include "Game.h"
#include "Mario.h"
#include "StatManager.h"

void Mario::HandleGrowing(const float dt)
{
	transformTimer.ProcessTimer(dt);
	if (!transformTimer.IsFinished())
	{
		return;
	}
	power = MarioPower::Big;
	if (isGrounded)
		state = MarioState::Idle;
	else
		state = MarioState::Jumping;
	transformTimer.SetIdle();
}

void Mario::HandleShrinking(float dt)
{
	transformTimer.ProcessTimer(dt);
	if (!transformTimer.IsFinished())
		return;

	power = MarioPower::Normal;
	invincibleTimer = Timer(MARIO_INVINCIBLE_TIME);
	invincibleTimer.Start();
	if (isGrounded)
		state = MarioState::Idle;
	else
		state = MarioState::Jumping;
	transformTimer.SetIdle();
}


void Mario::MarioDyingState(float dt)
{
	// As he goes up, this will slow his negative velocity until it hits its peak - 0.
	// Then it turns positive, pulling him down faster and faster.
	velocity.y += RUN_FALL_A * dt;
	position.y += velocity.y * dt;
	transformTimer.ProcessTimer(dt);

	if (transformTimer.IsFinished()) {
		transformTimer.SetIdle();

		if (StatManager::GetInstance()->GetLife() <= 0)
			Game::GetInstance()->IndicateSceneSwitch(GAME_OVER_SCENE, {});
		else
		{
			StatManager::GetInstance()->UndoValues();
			Game::GetInstance()->ReloadCurrentScene();
		}
	}
}

void Mario::MarioPullingFlag(float dt)
{
	isCollidable = false;

	if (position.y < slidingToYWinning)
	{
		position.y += 150 * dt;
	}
	else
	{
		if (!flagPoleFlipWaitTimer.IsTicking())
		{
			flagPoleFlipWaitTimer.Start();
			position.y = slidingToYWinning;
			isFacingRight = false;
			position.x += 16;
		}

		flagPoleFlipWaitTimer.ProcessTimer(dt);
		if (flagPoleFlipWaitTimer.IsFinished())
		{
			state = MarioState::WalkingToCastle;
			flagPoleFlipWaitTimer.SetIdle();
			isFacingRight = true;

			AudioManager::GetInstance()->PlaySFX(STAGE_CLEAR);
		}

	}
}

void Mario::MarioWalkingToCastle(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	isCollidable = true;

	if (abs(marioWinningMoveToPosition.x - position.x) > 2)
	{
		auto dir = (marioWinningMoveToPosition - position).Normalized();
		velocity.x = dir.x * 150.0f;
		velocity.y += 9000 * dt;
		Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);
	}
	else
	{
		isRendering = false;
	}
}
void Mario::MarioWaitingToBowser(float dt)
{
	velocity.x = 0;
	velocity.y = 0;
	if (!waitToBowserTimer.IsTicking())
	{
		waitToBowserTimer.Start();
	}
	waitToBowserTimer.ProcessTimer(dt);
	if (waitToBowserTimer.IsFinished())
	{
		waitToBowserTimer.SetIdle();
		state = MarioState::Jumping;
	}
}
void Mario::MarioEnteringPipe(float dt)
{
	isCollidable = false;
	// set render index to behind pipe
	renderIndex = -2;
	const auto& pipeRect = pipeData.zone;

	// move to position
	if (pipeData.enterDirection == Vector2Int::Up())
	{
		// snap x to middle of pipe
		position.x = static_cast<float>(pipeRect.left) + static_cast<float>(pipeRect.GetWidth()) / 2.0f - 8.0f;
		position.y = (std::max)(position.y, static_cast<float>(pipeData.moveTo.y));
	}
	if (pipeData.enterDirection == Vector2Int::Down())
	{
		// snap x to middle of pipe
		position.x = static_cast<float>(pipeRect.left) + static_cast<float>(pipeRect.GetWidth()) / 2.0f - 8.0f;
		position.y = (std::min)(position.y, static_cast<float>(pipeData.moveTo.y));
	}
	if (pipeData.enterDirection == Vector2Int::Left())
	{
		if (position.x < pipeData.zone.left)
		{
			isRendering = false;
		}

		position.x = (std::max)(position.x, static_cast<float>(pipeData.moveTo.x));
		position.y = static_cast<float>(pipeData.zone.bottom - GetBoundingBox().GetHeight());

	}
	if (pipeData.enterDirection == Vector2Int::Right())
	{
		if (position.x > pipeData.zone.right)
		{
			isRendering = false;
		}

		position.x = (std::min)(position.x, static_cast<float>(pipeData.moveTo.x));
		position.y = static_cast<float>(pipeData.zone.bottom - GetBoundingBox().GetHeight());

	}
	position += Vector2(pipeData.enterDirection) * 50.0f * dt;
}
