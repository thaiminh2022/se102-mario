#include "AssetIDs.h"
#include "AudioManager.h"
#include "Coin.h"
#include "FireballTrap.h"
#include "FlagPole.h"
#include "Flower.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Mario.h"
#include "Mushroom.h"
#include "NextLevelPortal.h"
#include "QuestionBlock.h"
#include "Star.h"
#include "StatManager.h"
#include "CollisionEvent.h"

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
			StatManager::GetInstance()->AddScore(100);
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
			AudioManager::GetInstance()->PlaySFX(AUDIOS::GOOMBA_STOMP);

			return true;

		}
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
		StatManager::GetInstance()->AddCoin(1);
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
			StatManager::GetInstance()->AddScore(1000);
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
		StatManager::GetInstance()->AddScore(1000);
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


	// DebugOutTitle(L"Score for flagpole: %f\n", score); //for debugging
	StatManager::GetInstance()->AddScore(score);

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


void Mario::OnCollisionWith(CollisionEvent* e)
{
	//DebugOut(L"NormalTransition %d, %d\n", e->normalizedDir.x, e->normalizedDir.y);

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
		if (OnCollisionWithPortal(e)) return;
		if (OnCollisionWithQuestionBlock(e)) return;
		if (OnCollisionWithCoin(e)) return;
		if (OnCollisionWithMushroom(e)) return;
		if (OnCollisionWithFlower(e)) return;
		if (OnCollisionWithStar(e)) return;
		if (OnCollisionWithFlagPole(e)) return;
	}
}

void Mario::OnNoCollision(float dt)
{
	position += velocity * dt;
	isGrounded = false;
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
		// got kill by enemy, bad

		state = MarioState::Dying;
		isCollidable = false; // Turn off hitboxes
		velocity.x = 0;
		velocity.y = -240.0f;
		//Mario will jump up a bit
		transformTimer = Timer(5.0f); // Time until we reset the level
		transformTimer.Start();
		AudioManager::GetInstance()->StopAll();
		AudioManager::GetInstance()->PlaySFX(MARIO_DIE);
		StatManager::GetInstance()->AddLife(-1);
	}
}