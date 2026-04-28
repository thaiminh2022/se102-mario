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
#include "StatManager.h"

#include <cmath>

#include "Debug.h"
#include "AudioManager.h"
#include "Coin.h"
#include "FontManager.h"
#include "Mushroom.h"
#include "NextLevelPortal.h"
#include "QuestionBlock.h"
#include "Fireball.h"
#include "FireballTrap.h"
#include "FlagPole.h"
#include "Flower.h"
#include "Star.h"

int Mario::goombaKilled = 0;
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

void Mario::OnMarioHit()
{
	if (!isInvincible)
	{
		if (power != MarioPower::Normal)
		{
			state = MarioState::Shrinking;
			AudioManager::GetInstance()->PlaySFX(PIPE_ENTER); // Original used pipe sound for power down
			isInvincible = true;
			transformTimer = Timer(MARIO_SHRINK_TIME);
			transformTimer.Start();
			return;
		}
		// got kill by goomba, bad
		state = MarioState::Dying;
		isCollidable = false; // Turn off hitboxes
		velocity.x = 0;
		velocity.y = -240.0f;
		//Mario will jump up a bit
		transformTimer = Timer(5.0f); // Time until we reset the level
		transformTimer.Start();
		AudioManager::GetInstance()->StopAll();
		AudioManager::GetInstance()->PlaySFX(MARIO_DIE);
	}
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

void Mario::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{

	if (state == MarioState::Dying)
	{
		// As he goes up, this will slow his negative velocity until it hits its peak - 0.
		// Then it turns positive, pulling him down faster and faster.
		velocity.y += RUN_FALL_A * dt;
		position.y += velocity.y * dt;

		transformTimer.ProcessTimer(dt);

		if (transformTimer.IsFinished()) {
			transformTimer.SetIdle();
			Game::GetInstance()->ReloadCurrentScene();
		}
		return;
	}
	if (state == MarioState::Growing)
	{
		if (HandleGrowing(dt))
			return;
	}
	if (state == MarioState::Shrinking)
	{
		HandleShrinking(dt);
		return;
	}

	if (state == MarioState::PullingFlag)
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
		return;
	}
	if (state == MarioState::WalkingToCastle)
	{
		isCollidable = true;

		if (abs(marioWinningMoveToPosition.x - position.x) > 2)
		{
			auto dir = (marioWinningMoveToPosition - position).Normalized();
			velocity.x = dir.x * 150.0f;
			velocity.y += 9000 * dt;
			DebugOutTitle(L"%f %f", dir.x, dir.y);
			Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);
		}
		else
		{
			isRendering = false;
		}
		return;
	}

	if (state == MarioState::EnteringPipe)
	{
		isCollidable = false;
		// set render index to behind pipe
		renderIndex = -2;
		const auto& pipeRect = pipeData.zone;
		
		// move to position
		if (pipeData.enterDirection == Vector2Int::Up())
		{
			// snap x to middle of pipe
			position.x = pipeRect.left + pipeRect.GetWidth() / 2 - 8.0f;
			position.y = std::max<float>(position.y, pipeData.moveTo.y);
		}
		if (pipeData.enterDirection == Vector2Int::Down())
		{
			// snap x to middle of pipe
			position.x = pipeRect.left + pipeRect.GetWidth() / 2 - 8.0f;
			position.y = std::min<float>(position.y, pipeData.moveTo.y);
		}
		if (pipeData.enterDirection == Vector2Int::Left())
		{
			if (position.x < pipeData.zone.left)
			{
				isRendering = false;
			}

			position.x = std::max<float>(position.x, pipeData.moveTo.x);
			position.y = pipeData.zone.bottom - GetBoundingBox().GetHeight();

		}
		if (pipeData.enterDirection == Vector2Int::Right())
		{
			if (position.x > pipeData.zone.right)
			{
				isRendering = false;
			}

			position.x = std::min<float>(position.x, pipeData.moveTo.x);
			position.y = pipeData.zone.bottom - GetBoundingBox().GetHeight();

		}
		position += Vector2(pipeData.enterDirection) * 50.0f * dt;
		return;
	}

	if (state == MarioState::ExitingPipe)
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
		return;
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

bool Mario::HandleGrowing(const float dt)
{
	transformTimer.ProcessTimer(dt);
	if (!transformTimer.IsFinished())
		return true;
	power = MarioPower::Big;
	if (isGrounded)
		state = MarioState::Idle;
	else
		state = MarioState::Jumping;
	transformTimer.SetIdle();
	return false;
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

void Mario::ApplyGravityAndClamp(float dt)
{
	const auto input = InputManager::GetInstance();


	// APPLY GRAVITY
	velocity.y += fallAcc * dt;

	// Y-axis clamping
	velocity.y = min(velocity.y, MAX_FALL);
	velocity.y = max(velocity.y, -MAX_FALL);

	// Absolute X-axis clamping
	velocity.x = min(velocity.x, MAX_RUN);
	velocity.x = max(velocity.x, -MAX_RUN);

	// Clamp back to Walk speed if Shift is released
	if (isGrounded)
	{
		if (velocity.x > MAX_WALK && !input->IsKeyDown(VK_SHIFT)) velocity.x = MAX_WALK;
		if (velocity.x < -MAX_WALK && !input->IsKeyDown(VK_SHIFT)) velocity.x = -MAX_WALK;
	}
}

void Mario::UpdateFacingDirection()
{
	const auto input = InputManager::GetInstance();
	if (input->IsKeyDown('A') && !input->IsKeyDown('D') && isGrounded && state != MarioState::Ducking)
	{
		isFacingRight = false;
	}
	else if (input->IsKeyDown('D') && !input->IsKeyDown('A') && isGrounded && state != MarioState::Ducking)
	{
		isFacingRight = true;
	}
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

void Mario::OnCollisionWithFireballTrap(vector<GameObject*>& coObjects)
{

	for (const auto& other : coObjects)
	{
		const auto fireballTrap = dynamic_cast<FireballTrap*>(other);
		if (fireballTrap == nullptr)
			continue;

		if (!fireballTrap->GetBoundingBox().IsColliding(GetBoundingBox()))
			continue;

		if (!fireballTrap->IsHitSmallBalls(GetBoundingBox()))
			continue;

		OnMarioHit();
	}
}

int Mario::GetFlagBonusScore(float touchingHeight) const
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

void Mario::OnNoCollision(float dt)
{
	position += velocity * dt;
	isGrounded = false;
}

bool Mario::OnCollisionWithGoomba(const CollisionEvent* e)
{
	// resolve object collision
	const auto goomba = dynamic_cast<Goomba*>(e->otherObject);

	if (goomba != nullptr)
	{
		if (goomba->GetState() == GoombaState::Dead)
			return false;

		if (e->normalizedDir.y == -1)
		{
			// jump on head

			velocity.y = -240.0f;
			state = MarioState::Jumping;

			goomba->SetState(GoombaState::Dead);

			goombaKilled++;
			StatManager::AddScore(100);
			AudioManager::GetInstance()->PlaySFX(GOOMBA_STOMP);
			return true;
		}
		else {
			// dead
			OnMarioHit();
			return true;
		}
	}
	return false;
}

bool Mario::OnCollisionWithPortal(const CollisionEvent* e)
{
	const auto portal = dynamic_cast<NextLevelPortal*>(e->otherObject);
	if (portal != nullptr)
	{
		portal->RequestNextLevel();
		return true;
	}
	return false;
}

bool Mario::OnCollisionWithQuestionBlock(const CollisionEvent* e)
{
	const auto questionBlock = dynamic_cast<QuestionBlock*>(e->otherObject);
	if (questionBlock != nullptr)
	{
		if (e->normalizedDir.y == -1)
		{
			isGrounded = true;
			return true;
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
		return true;
	}
	return false;
}

bool Mario::OnCollisionWithCoin(const CollisionEvent* e)
{
	const auto coin = dynamic_cast<Coin*>(e->otherObject);
	if (coin != nullptr)
	{
		coin->SetState(CoinState::Collected);
		coinCollected++;
		AudioManager::GetInstance()->PlaySFX(MARIO_COLLECT_COIN);
		return true;
	}
	return false;
}

bool Mario::OnCollisionWithMushroom(const CollisionEvent* e)
{
	const auto mushroom = dynamic_cast<Mushroom*>(e->otherObject);
	if (mushroom != nullptr)
	{
		mushroom->SetState(CollectableItemState::Collected);
		AudioManager::GetInstance()->PlaySFX(MARIO_POWERUP);

		if (power == MarioPower::Normal)
		{
			state = MarioState::Growing;
			transformTimer = Timer(MARIO_GROW_TIME);
			StatManager::AddScore(1000);
			transformTimer.Start();
			// add some pushback so player won't fall off the ground
			position.y -= 17;
			return true;
		}
	}
	return false;
}

bool Mario::OnCollisionWithFlower(CollisionEvent* e)
{
	const auto flower = dynamic_cast<Flower*>(e->otherObject);
	if (flower != nullptr)
	{
		if (power == MarioPower::Normal)
		{
			state = MarioState::Growing; // If collect a flower while small, grow to big
			transformTimer = Timer(MARIO_GROW_TIME);
			transformTimer.Start();
			// add some pushback so player won't fall off the ground
			position.y -= 17;
		}
		else if (power == MarioPower::Big)
		{
			power = MarioPower::Fire; // Instantly power up to Fire if already Big
		}

		flower->SetState(CollectableItemState::Collected);
		StatManager::AddScore(1000);
		AudioManager::GetInstance()->PlaySFX(MARIO_POWERUP);
		return true;
	}
	return false;
}

bool Mario::OnCollisionWithStar(const CollisionEvent* e)
{
	const auto star = dynamic_cast<Star*>(e->otherObject);
	if (star != nullptr)
	{
		star->SetState(CollectableItemState::Collected);
		auto audio = AudioManager::GetInstance();


		isInvincible = true;
		invincibleTimer = Timer(15);
		invincibleTimer.Start();

		audio->PauseMusic();
		audio->PlaySFX(MARIO_POWERUP);
		AudioManager::GetInstance()->Play(INVINCIBILITY_THEME, false, []()
			{
				AudioManager::GetInstance()->ResumeMusic();
			});
		return true;
	}
	return false;
}

bool Mario::OnCollisionWithFlagPole(const CollisionEvent* collisionEvent)
{
	const auto flagPole = dynamic_cast<FlagPole*>(collisionEvent->otherObject);
	if (flagPole == nullptr)
		return false;


	float score = 0;
	float bottom = flagPole->GetBoundingBox().bottom;
	float touchingPoint = position.y + (power == MarioPower::Normal ? 16 : 32); // Mario's feet position
	float touchingHeight = bottom - touchingPoint;

	score = GetFlagBonusScore(touchingHeight);


	DebugOutTitle(L"Score for flagpole: %f\n", score); //for debugging
	StatManager::AddScore(score);

	AudioManager::GetInstance()->StopAll();
	AudioManager::GetInstance()->PlaySFX(FLAG_PULL);

	// set state to flag sliding
	velocity = Vector2(0, 0);
	state = MarioState::PullingFlag;
	// make mario face right
	isFacingRight = true;

	// set flagPole to collided
	flagPole->SetFlagMove();
	// initiate sliding down
	auto snapPosition = flagPole->GetSnapPosition();
	position.x = snapPosition.x;
	position.y = max(position.y, snapPosition.y);

	float offset = power == MarioPower::Normal ? 16 : 32;

	slidingToYWinning = flagPole->GetBoundingBox().bottom - offset;
	marioWinningMoveToPosition = flagPole->GetMoveToPosition();
	return true;
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
	return MARIO_IDLE_ANIM_ID;
}

void Mario::LoadSpriteAndAnimation()
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

	sprites->Add(MARIO_FLAG_PULL_SPRITE_1, 112, 0, 127, 15, marioTex);
	sprites->Add(MARIO_FLAG_PULL_SPRITE_2, 128, 0, 143, 15, marioTex);

	sprites->Add(MARIO_GROWBIG_SPRITE_1, 0, 48, 15, 79, marioTex);
	sprites->Add(MARIO_GROWBIG_SPRITE_2, 16, 48, 31, 79, marioTex);
	sprites->Add(MARIO_GROWBIG_SPRITE_3, 0, 16, 15, 47, marioTex);

	// idle anim
	auto anim = new Animation(300);
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

	// grow to big anim
	anim = new Animation(150);
	anim->Add(MARIO_GROWBIG_SPRITE_1, 100);
	anim->Add(MARIO_GROWBIG_SPRITE_2, 100);
	anim->Add(MARIO_GROWBIG_SPRITE_1, 100);
	anim->Add(MARIO_GROWBIG_SPRITE_2, 100);
	anim->Add(MARIO_GROWBIG_SPRITE_3, 100);
	anim->Add(MARIO_GROWBIG_SPRITE_2, 100);
	anim->Add(MARIO_GROWBIG_SPRITE_3, 100);
	anims->Add(MARIO_GROWBIG_ANIM_ID, anim);

	// flag pull anim
	anim = new Animation(150);
	anim->Add(MARIO_FLAG_PULL_SPRITE_1, 150);
	anim->Add(MARIO_FLAG_PULL_SPRITE_2, 150);
	anims->Add(MARIO_FLAG_PULL_ANIM_ID, anim);

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

	sprites->Add(MARIO_SHRINK_SPRITE_1, 32, 48, 47, 79, marioTex);
	sprites->Add(MARIO_SHRINK_SPRITE_2, 48, 48, 63, 79, marioTex);
	sprites->Add(MARIO_SHRINK_SPRITE_3, 64, 48, 79, 79, marioTex);

	sprites->Add(MARIO_BIG_FLAG_PULL_SPRITE_1, 112, 16, 127, 47, marioTex);
	sprites->Add(MARIO_BIG_FLAG_PULL_SPRITE_2, 128, 16, 143, 47, marioTex);


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
	anim->Add(MARIO_SHRINK_SPRITE_1, 50);
	anim->Add(MARIO_SHRINK_SPRITE_3, 50);
	anim->Add(MARIO_SHRINK_SPRITE_1, 50);
	anim->Add(MARIO_SHRINK_SPRITE_3, 50);
	anim->Add(MARIO_SHRINK_SPRITE_1, 50);
	anim->Add(MARIO_SHRINK_SPRITE_3, 50);
	anim->Add(MARIO_SHRINK_SPRITE_1, 50);
	anim->Add(MARIO_SHRINK_SPRITE_3, 50);
	anim->Add(MARIO_SHRINK_SPRITE_2, 50);
	anim->Add(MARIO_SHRINK_SPRITE_3, 50);
	anim->Add(MARIO_SHRINK_SPRITE_2, 50);
	anim->Add(MARIO_SHRINK_SPRITE_3, 50);
	anim->Add(MARIO_SHRINK_SPRITE_2, 50);
	anim->Add(MARIO_SHRINK_SPRITE_3, 50);
	anim->Add(MARIO_SHRINK_SPRITE_2, 50);
	anims->Add(MARIO_SHRINK_ANIM_ID, anim);

	//flag pull anim
	anim = new Animation(150);
	anim->Add(MARIO_BIG_FLAG_PULL_SPRITE_1, 150);
	anim->Add(MARIO_BIG_FLAG_PULL_SPRITE_2, 150);
	anims->Add(MARIO_BIG_FLAG_PULL_ANIM_ID, anim);

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

	sprites->Add(MARIO_FIRE_FIRE_SPRITE_1, 16, 80, 31, 111, marioTex);
	// Reusing the run sprite for firing since it's the same pose
	sprites->Add(MARIO_FIRE_FLAG_PULL_SPRITE_1, 112, 80, 127, 111, marioTex);
	sprites->Add(MARIO_FIRE_FLAG_PULL_SPRITE_2, 128, 80, 143, 111, marioTex);

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

	//flag pull anim
	anim = new Animation(150);
	anim->Add(MARIO_FIRE_FLAG_PULL_SPRITE_1, 150);
	anim->Add(MARIO_FIRE_FLAG_PULL_SPRITE_2, 150);
	anims->Add(MARIO_FIRE_FLAG_PULL_ANIM_ID, anim);
}

void Mario::OnCollisionWith(CollisionEvent* e)
{
	//DebugOut(L"Normal %d, %d\n", e->normalizedDir.x, e->normalizedDir.y);

	if (e->IsTileCollision())
	{
		if (e->otherTile->type == CollisionTileType::Death)
		{
			OnMarioHit();
			return;
		}

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
		if (OnCollisionWithGoomba(e)) return;
		if (OnCollisionWithPortal(e)) return;
		if (OnCollisionWithQuestionBlock(e)) return;
		if (OnCollisionWithCoin(e)) return;
		if (OnCollisionWithMushroom(e)) return;
		if (OnCollisionWithFlower(e)) return;
		if (OnCollisionWithStar(e)) return;
		if (OnCollisionWithFlagPole(e)) return;
	}
}
