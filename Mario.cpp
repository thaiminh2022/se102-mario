#include "Mario.h"

#include "Animation.h"
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
	power = MarioPower::Big;
	fireCooldownTimer = Timer(MARIO_TIME_BTW_FIRE);
	fireCooldownTimer.Start();
	transformTimer = Timer(MARIO_GROW_TIME);
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
	}
	if (pipeExitingData.dir == Vector2Int::Left())
	{

	}
	if (pipeExitingData.dir == Vector2Int::Right())
	{
	}

	Game::GetInstance()->GetCamera()->SetPosition(position.x, position.y);

}

void Mario::MarioExitingPipe(float dt)
{
	isCollidable = false;
	//isRendering = false;
	renderIndex = -2;
		
	const auto& pipeRect = pipeExitingData.returnZone;

	position += Vector2(pipeExitingData.dir) * 50.0f * dt;

	if (pipeExitingData.dir == Vector2Int::Up())
	{
		if (position.y < pipeExitingData.moveTo.y)
		{
			state = MarioState::Idle;
			renderIndex = 0;
			isRendering = true;
			isCollidable = true;
		}
	}
	if (pipeExitingData.dir == Vector2Int::Down())
	{
	}
	if (pipeExitingData.dir == Vector2Int::Left())
	{

	}
	if (pipeExitingData.dir == Vector2Int::Right())
	{
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
	default:
		break;
	}


	if (isInvincible)
	{
		invincibleTimer.ProcessTimer(dt);
		if (invincibleTimer.IsFinished())
		{
			isInvincible = false;
			invincibleTimer.SetIdle();
			if (power == MarioPower::StarmanSmall || power == MarioPower::StarmanBig) {
				power = MarioPower::Normal;//currently reset to normal. Will change later
			}
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

	HandleJump(dt);
	HandleShootFireball(dt, coObjects, ctx);
	ApplyGravityAndClamp(dt);


	// UPDATE STATE & FACING DIRECTION

	// Update facing direction based on player input and only apply if grounded to prevent mid-air direction change
	UpdateFacingDirection();
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




