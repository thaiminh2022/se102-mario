#include "QuestionBlock.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "AudioManager.h"
#include "BrickExplode.h"
#include "Coin.h"
#include "Flower.h"
#include "Game.h"
#include "Goomba.h"
#include "Helper.h"
#include "Mario.h"
#include "Mushroom.h"
#include "Sprites.h"
#include "Star.h"
#include "Texture.h"
#include "Textures.h"
#include "StatManager.h"
#include "Koopa.h"

void QuestionBlock::SetState(const QuestionBlockState newState)
{
	AudioManager::GetInstance()->PlaySFX(MARIO_BUMP);


	if (state == QuestionBlockState::Blocked)
	{
		return;
	}

	state = newState;
	if (newState == QuestionBlockState::Opened)
	{
		moveUpTimer.Start();
		isHidden = false;
	}
}

void QuestionBlock::Render()
{
	if (isHidden)
		return;

	int animID = BLOCK_EMPTY_ANIM_ID;
	if (state != QuestionBlockState::Blocked)
	{
		animID = isBrick ? BRICK_IDLE_ANIM_ID : QUESTION_BLOCK_IDLE_ANIM_ID;
	}


	const auto anim = Animations::GetInstance()->Get(animID);
	float renderX, renderY;

	Game::GetInstance()
		->GetCamera()
		->WorldToScreen(renderPosition.x, renderPosition.y, renderX, renderY);
	anim->Render(round(renderX), round(renderY), false, false);
}

void QuestionBlock::CheckHitBounce(vector<GameObject*>& coObjects, SceneContext* ctx) const
{
	auto sm = StatManager::GetInstance();
	auto mario = ctx->mario;
	for (auto& go : coObjects)
	{
		if (!go->GetBoundingBox().IsColliding(bounceCheckBox))
			continue;

		auto mushroom = dynamic_cast<Mushroom*>(go);
		if (mushroom != nullptr)
		{
			float pushAmount;
			auto pushDir = bounceCheckBox.GetPushDir(mushroom->GetBoundingBox(), pushAmount);
			if (pushDir.x != 0)
			{
				mushroom->SetMoveDirX(pushDir.x);
			}
		}
		auto goomba = dynamic_cast<Goomba*>(go);
		if (goomba != nullptr)
		{
			if (goomba->GetState() != GoombaState::Moving)
				continue;

			goomba->SetState(GoombaState::DeadUpsideDown);
			sm->AddEnemyKillScore(mario->GetEnemyKilledOnSequenceCount(), this->position);
		}
		auto koopa = dynamic_cast<Koopa*>(go);
		if (koopa != nullptr)
		{
			if (koopa->GetForm() != KoopaForm::Winged)
				koopa->SetState(KoopaState::DeadUpsideDown);
		}
	}
}

void QuestionBlock::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (state == QuestionBlockState::Blocked)
		return;
	auto sm = StatManager::GetInstance();
	if (state == QuestionBlockState::Opened)
	{
		if (!spawnInternalItem)
		{
			CheckHitBounce(coObjects, ctx);

			if (drop == BlockDropType::Coin)
			{
				sm->AddScore(200, this->position);
				if (ctx->addPointPopup != nullptr)
				{
					ctx->addPointPopup(Vector2(startPosition.x, startPosition.y - 16.0f), 200);
				}
				ctx->addObject(new Coin(
					Vector2Int(
						static_cast<int>(round(startPosition.x)),
						static_cast<int>(round(startPosition.y - 8))
					), biome,
					CoinState::CollectedFromQuestionBox)
				);
				StatManager::GetInstance()->AddCoin(1);
			}
			else if (drop == BlockDropType::JewDestroyer)
			{
				if (ctx->mario == nullptr)
					return;

				auto power = ctx->mario->GetPowerLevel();

				if (power == MarioPower::Normal)
				{
					ctx->addObject(new Mushroom(position, biome));
				}
				else
				{
					ctx->addObject(new Flower(position));
				}

			}
			else if (drop == BlockDropType::Starman)
			{
				ctx->addObject(new Star(startPosition));
			}
			spawnInternalItem = true;
		}

		moveUpTimer.ProcessTimer(dt);
		if (!moveUpTimer.IsFinished())
		{
			renderPosition.y -= 125.0f * dt;

		}
		else
		{
			if (renderPosition.y < startPosition.y)
			{
				renderPosition.y += 9000.0f * dt * dt;

			}
			else
			{
				moveUpTimer.SetIdle();
				renderPosition.y = startPosition.y;
				state = QuestionBlockState::Blocked;
			}
		}
	}

	if (state == QuestionBlockState::Break)
	{
		CheckHitBounce(coObjects, ctx);
		auto debris = new BrickExplode(startPosition, biome);
		ctx->addObject(debris);
		isDeleted = true;
		state = QuestionBlockState::Blocked;
	}

}

QuestionBlock::QuestionBlock(Vector2Int startPos, BlockDropType drop, BiomeType biome, bool isBrick, bool isHidden) : GameObject(startPos)
{
	this->biome = biome;

	state = QuestionBlockState::Closed;
	const auto t = Textures::GetInstance()->Get(ChooseBlocksId(biome));
	const auto sp = Sprites::GetInstance();
	const auto anims = Animations::GetInstance();


	// question block
	if (!isBrick && !anims->Contains(QUESTION_BLOCK_IDLE_ANIM_ID))
	{
		sp->Add(QUESTION_BLOCK_IDLE_SPRITE_1, 0, 0, 15, 15, t);
		sp->Add(QUESTION_BLOCK_IDLE_SPRITE_2, 16, 0, 31, 15, t);
		sp->Add(QUESTION_BLOCK_IDLE_SPRITE_3, 32, 0, 47, 15, t);

		// idle
		auto anim = new Animation(200);
		anim->Add(QUESTION_BLOCK_IDLE_SPRITE_1);
		anim->Add(QUESTION_BLOCK_IDLE_SPRITE_2);
		anim->Add(QUESTION_BLOCK_IDLE_SPRITE_3);
		anims->Add(QUESTION_BLOCK_IDLE_ANIM_ID, anim);
	}

	if (isBrick && !anims->Contains(BRICK_IDLE_ANIM_ID))
	{
		// brick
		sp->Add(BRICK_IDLE_SPRITE_1, 0, 16, 15, 31, t);

		auto anim = new Animation(0);
		anim->Add(BRICK_IDLE_SPRITE_1);
		anims->Add(BRICK_IDLE_ANIM_ID, anim);
	}

	if (!anims->Contains(BLOCK_EMPTY_ANIM_ID))
	{
		// blocked
		sp->Add(BLOCK_EMPTY_SPRITE_1, 48, 0, 63, 15, t);
		auto anim = new Animation(0);
		anim->Add(BLOCK_EMPTY_SPRITE_1);
		anims->Add(BLOCK_EMPTY_ANIM_ID, anim);
	}

	spawnInternalItem = false;
	moveUpTimer = Timer(0.1f);

	this->drop = drop;
	this->isBrick = isBrick;
	this->isHidden = isHidden;

	startPosition = position;
	renderPosition = position;

	bounceCheckBox = Rect::FromXYWH(startPos.x, startPos.y - 16, 16, 16);
}



Rect QuestionBlock::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 16, 16);
}

