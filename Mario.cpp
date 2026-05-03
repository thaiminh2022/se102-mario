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

bool Mario::OnCollisionWithKoopa(const CollisionEvent* e)
{
	const auto koopa = dynamic_cast<Koopa*>(e->otherObject);
	if (koopa != nullptr)
	{
		if (koopa->GetState() == KoopaState::Dead)
			return false;

		if (e->normalizedDir.y == -1)
		{
			// jump on head
			velocity.y = -240.0f;
			state = MarioState::Jumping;
			if (koopa->GetForm() == KoopaForm::Winged)
			{
				koopa->SetForm(KoopaForm::Normal);
			}
			else if (koopa->GetForm() == KoopaForm::Normal)
			{
				koopa->SetForm(KoopaForm::HiddingInShell);
				koopa->SetState(KoopaState::NotMoving);
			}
			else if (koopa->GetForm() == KoopaForm::HiddingInShell)
			{
				if (koopa->GetState() == KoopaState::NotMoving)
					koopa->SetState(KoopaState::Moving);
				else
				{
					koopa->SetState(KoopaState::Dead);
					koopaKilled++;
				}
			}
			return true;
		}
		OnMarioHit();
		return true;
	}
	return false;
}

bool Mario::OnCollisionWithBowser(const CollisionEvent* e)
{
	const auto bowser = dynamic_cast<Bowser*>(e->otherObject);
	const auto bowserFireBullet = dynamic_cast<BowserFireBullet*>(e->otherObject);
	const auto bowserHammer = dynamic_cast<BowserHammer*>(e->otherObject);
	if (bowser != nullptr )
	{
		OnMarioHit();
		return true;
	}
	else if (bowserFireBullet != nullptr)
	{
		if (bowserFireBullet->GetState() == BowserFireBulletState::Discarded)
			return false;
		bowserFireBullet->SetState(BowserFireBulletState::Discarded);
		OnMarioHit();
		return true;
	}
	else if (bowserHammer != nullptr)
	{
		if (bowserHammer->GetState() == BowserHammerState::Discarded)
			return false;
		bowserHammer->SetState(BowserHammerState::Discarded);
		OnMarioHit();
		return true;
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
		if (OnCollisionWithKoopa(e)) return;
		if (OnCollisionWithBowser(e)) return;
		if (OnCollisionWithPortal(e)) return;
		if (OnCollisionWithQuestionBlock(e)) return;
		if (OnCollisionWithCoin(e)) return;
		if (OnCollisionWithMushroom(e)) return;
		if (OnCollisionWithFlower(e)) return;
		if (OnCollisionWithStar(e)) return;
		if (OnCollisionWithFlagPole(e)) return;
	}
}
