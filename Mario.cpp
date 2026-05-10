#include "Mario.h"
#include "AssetIDs.h"
#include "Collision.h"
#include "Game.h"
#include "GameObject.h"
#include "Goomba.h"
#include "InputManager.h"
#include "Rect.h"
#include "Scene.h"
#include <vector>
#include "AudioManager.h"
#include "FontManager.h"
#include "Fireball.h"

int Mario::GetFireBallCount(const vector<GameObject*>& coObjects) const
{
	int count = 0;

	for (const auto& co : coObjects)
	{
		if (IsDeleted(co))
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
	isInWater = false;
	isRendering = true;
	slidingToYWinning = 0;
	flagPoleFlipWaitTimer = Timer(1);
	isGrounded = false;
	isInvincible = false;
	isCollidable = true;
	isFacingRight = true;
	velocity.x = 0.0f;
	velocity.y = 0.0f;
	state = MarioState::Idle;
	power = MarioPower::Normal;
	lastPower = MarioPower::Normal;
	fireCooldownTimer = Timer(MARIO_FIRE_INTERVAL);
	fireCooldownTimer.Start();
	transformTimer = Timer(MARIO_GROW_TIME);
	starmanTimer = Timer(STARMAN_INVINCIBLE_TIME);
	invincibleTimer = Timer(MARIO_INVINCIBLE_TIME);
	enemySequenceKilledCount = 0;
	LoadSpriteAndAnimation();
}

void Mario::SetEnterPipe(const PipeData& pipe)
{
	state = MarioState::EnteringPipe;
	pipeData = pipe;
}

void Mario::SetExitPipe(const MarioPipeCtx& returnPipeData)
{
	state = MarioState::ExitingPipe;
	AudioManager::GetInstance()->ResumeMusic();
	AudioManager::GetInstance()->PlaySFX(PIPE_ENTER);
	pipeExitingData = returnPipeData;
	const auto& pipeRect = pipeExitingData.returnZone;
	if (pipeExitingData.dir == Vector2Int::Up())
	{
		position.x = pipeRect.left + 8;
		position.y = pipeRect.bottom;
	}
	if (pipeExitingData.dir == Vector2Int::Down())
	{
		position.x = pipeRect.left + 8;
		position.y = pipeRect.top - GetBoundingBox().GetHeight();
	}
	if (pipeExitingData.dir == Vector2Int::Left())
	{
		position.x = pipeRect.right;
		position.y = pipeRect.bottom - GetBoundingBox().GetHeight();
	}
	if (pipeExitingData.dir == Vector2Int::Right())
	{
		position.x = pipeRect.left - GetBoundingBox().GetWidth();
		position.y = pipeRect.bottom - GetBoundingBox().GetHeight();
	}

	Game::GetInstance()->GetCamera()->SetPosition(position.x - 64, 0);

}

void Mario::SetIsInWater(const bool newIsInWater)
{
	if (isInWater == newIsInWater)
		return;

	isInWater = newIsInWater;

	if (isInWater)
	{
		fallAcc = 180.0f;
		velocity.y = min(velocity.y, 190.0f);
	}
	else
	{
		fallAcc = STOP_FALL;
	}
}


void Mario::MarioExitingPipe(float dt)
{
	isCollidable = false;
	renderIndex = -2;
	isRendering = true;

	position += Vector2(pipeExitingData.dir) * 50.0f * dt;

	bool finished = false;
	if (pipeExitingData.dir == Vector2Int::Up())
	{
		finished = position.y < pipeExitingData.moveTo.y;
	}
	if (pipeExitingData.dir == Vector2Int::Down())
	{
		finished = position.y > pipeExitingData.moveTo.y;
	}
	if (pipeExitingData.dir == Vector2Int::Left())
	{
		finished = position.x < pipeExitingData.moveTo.x;
	}
	if (pipeExitingData.dir == Vector2Int::Right())
	{
		finished = position.x > pipeExitingData.moveTo.x;
	}

	if (finished)
	{
		position = Vector2(pipeExitingData.moveTo);
		ResetRender();
		ResetState();
	}
}

void Mario::ClampMario()
{
	auto cam = Game::GetInstance()->GetCamera();
	if (position.x < cam->GetX())
	{
		velocity.x = 0;
		state = MarioState::Idle;
		position.x = cam->GetX();
	}
	if (position.y < cam->GetY())
	{
		velocity.y = 0;
		state = MarioState::Idle;
		position.y = cam->GetY();
	}
}



void Mario::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{

	switch (state)
	{
	case MarioState::PullingFlag:
		MarioPullingFlag(dt);
		return;
	case MarioState::WalkingToCastle:
		MarioWalkingToCastle(dt, coObjects, ctx);
		return;
	case MarioState::EnteringPipe:
		MarioEnteringPipe(dt);
		return;
	case MarioState::ExitingPipe:
		MarioExitingPipe(dt);
		return;
	case MarioState::Dying:
		MarioDyingState(dt);
		return;
	case MarioState::Growing:
		HandleGrowing(dt);
		return;
	case MarioState::Shrinking:
		HandleShrinking(dt);
		return;
	case MarioState::Idle:
	case MarioState::Walking:
	case MarioState::Running:
	case MarioState::Skidding:
	case MarioState::Jumping:
	case MarioState::Ducking:
	case MarioState::Firing:
		break;
	}


	if (isInvincible)
	{
		invincibleTimer.ProcessTimer(dt);
		if (invincibleTimer.IsFinished())
		{
			invincibleTimer.SetIdle();
			isInvincible = false;
		}
	}
	if (power == MarioPower::StarmanSmall || power == MarioPower::StarmanBig) {
		starmanTimer.ProcessTimer(dt);
		if (starmanTimer.IsFinished())
		{
			starmanTimer.SetIdle();
			power = lastPower;//currently reset to normal. Will change later
		}
	}

	auto input = InputManager::GetInstance();
	if (isGrounded)
	{
		WhileGrounded(dt);
	}
	else
	{
		WhileOnAir(dt);
	}

	// orders matters
	HandleSwim(dt);
	HandleJump(dt);
	HandleShootFireball(dt, coObjects, ctx);
	ApplyGravityAndClamp(dt);
	UpdateFacingDirection();

	ClampMario();
	RouteAnimationState();


	// FOR NOW, FIREBALL TRAP WILL BE CHECK IN UPDATE
	// WE SHOULD HAVE A BETTER SOLUTION
	OnCollisionWithFireballTrap(coObjects);
	Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);
}


int Mario::GetFlagBonusScore(float touchingHeight)
{
	int score = 0;
	// 0 - 17 pixels high : 100 extra points-- - 1 BLOCKWIDTH up from floor + blockwidth
	// 18 - 57 pixels high : 400 extra points-- - 2 - 3 BLOCKWIDTH
	// 58 - 81 pixels high : 800 extra points-- 3 - 4 BLOCKWIDTH 
	// 82 - 127 pixels high : 2000 extra points-- 4 - 5 BLOCKWIDTH
	// 128 - 153 pixels high : 4000 extra points -- above

	if (touchingHeight >= 0 && touchingHeight <= 17) {
		score = 100;
	}
	else if (touchingHeight > 17 && touchingHeight <= 57) {
		score = 400;
	}
	else if (touchingHeight > 57 && touchingHeight <= 81) {
		score = 800;
	}
	else if (touchingHeight > 81 && touchingHeight <= 127) {
		score = 2000;
	}
	else {
		score = 4000;
	}
	return score;
}

void Mario::OnHittingGround() {
	if (!isGrounded)
	{
		isGrounded = true;
		enemySequenceKilledCount = 0;
	}
}

Rect Mario::GetBoundingBox()
{
	RectF r;
	if (power == MarioPower::Normal || power == MarioPower::StarmanSmall)
	{
		r.top = position.y;
		r.left = position.x + 1;
		r.bottom = position.y + 16;
		r.right = position.x + 14;
	}
	else if (power == MarioPower::Big || power == MarioPower::Fire || power == MarioPower::StarmanBig)
	{
		r.top = position.y;
		r.left = position.x + 2;
		r.bottom = position.y + 32;
		r.right = position.x + 14;
	}
	return r;
}




