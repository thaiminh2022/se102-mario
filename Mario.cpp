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
#include <algorithm>
#include <vector>
#include "AudioManager.h"
#include "Debug.h"
#include "FontManager.h"
#include "Fireball.h"

int Mario::goombaKilled = 0;
int Mario::koopaKilled = 0;
int Mario::coinCollected = 0;

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
	power = MarioPower::Normal;
	fireCooldownTimer = Timer(MARIO_TIME_BTW_FIRE);
	fireCooldownTimer.Start();
	transformTimer = Timer(MARIO_GROW_TIME);
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

bool Mario::CheckMarioFalloffMap()
{
	auto vpHeight = Game::GetInstance()->GetBackBufferHeight();

	// Check Mario fall off map
	constexpr float marioMaxHeightOffset = 32.0f;
	if (position.y > vpHeight + marioMaxHeightOffset && state != MarioState::Dying)
	{
		isInvincible = false;
		invincibleTimer.Stop();
		state = MarioState::Dying;
		OnMarioHit();
		return true;
	}
	return false;
}

void Mario::ClampMarioXToCameraX()
{
	auto cam = Game::GetInstance()->GetCamera();
	if (position.x < cam->GetX())
	{
		velocity.x = 0;
		state = MarioState::Idle;
		position.x = cam->GetX();
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
			isInvincible = false;
			invincibleTimer.SetIdle();
		}
	}



	if (CheckMarioFalloffMap())
	{
		return;
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
	UpdateFacingDirection();
	
	ClampMarioXToCameraX();
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



Rect Mario::GetBoundingBox()
{
	RectF r;
	if (power == MarioPower::Normal)
	{
		r.top = position.y;
		r.left = position.x + 1;
		r.bottom = position.y + 16;
		r.right = position.x + 14;
	}
	else if (power == MarioPower::Big || power == MarioPower::Fire)
	{
		r.top = position.y;
		r.left = position.x + 2;
		r.bottom = position.y + 32;
		r.right = position.x + 14;
	}
	return r;
}




