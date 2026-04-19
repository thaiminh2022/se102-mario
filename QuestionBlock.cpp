#include "QuestionBlock.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "AudioManager.h"
#include "BrickExplode.h"
#include "Coin.h"
#include "Flower.h"
#include "Game.h"
#include "Goomba.h"
#include "Mario.h"
#include "Mushroom.h"
#include "Sprites.h"
#include "Star.h"
#include "Texture.h"
#include "Textures.h"

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

	int animID = BLOCK_OVERWORLD_BLOCKED_ANIM_ID;
	if (state != QuestionBlockState::Blocked)
	{
		animID = isBrick ? BRICK_OVERWORLD_IDLE_ANIM_ID : QUESTION_BLOCK_OVERWORLD_IDLE_ANIM_ID;
	}


	const auto anim = Animations::GetInstance()->Get(animID);
	float renderX, renderY;

	Game::GetInstance()
	->GetCamera()
	->WorldToScreen(renderPosition.x, renderPosition.y, renderX, renderY);
	anim->Render(round(renderX), round(renderY), false, false);
}

void QuestionBlock::CheckHitBounce(vector<GameObject*>& coObjects) const
{
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
			goomba->SetState(GoombaState::DeadUpsideDown);
		}
	}
}

void QuestionBlock::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (state == QuestionBlockState::Blocked)
		return;

	if (state == QuestionBlockState::Opened)
	{
		if (!spawnInternalItem)
		{
			CheckHitBounce(coObjects);

			if (drop == BlockDropType::Coin)
			{
				ctx->addObject(new Coin(
					Vector2Int(
						static_cast<int>(round(startPosition.x)), 
						static_cast<int>(round(startPosition.y - 8))
					),
					CoinState::CollectedFromQuestionBox)
				);
			}
			else if (drop == BlockDropType::JewDestroyer)
			{
				if (ctx->mario == nullptr)
					return;

				auto power = ctx->mario->GetPowerLevel();

				if (power == MarioPower::Normal)
				{
					ctx->addObject(new Mushroom(position));
				}else
				{
					ctx->addObject(new Flower(position));
				}

			}else if (drop == BlockDropType::Starman)
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
		CheckHitBounce(coObjects);
		auto debris = new BrickExplode(startPosition);
		ctx->addObject(debris);
		isDeleted = true;
		state = QuestionBlockState::Blocked;
	}
	
}

QuestionBlock::QuestionBlock(const Vector2Int startPos, const BlockDropType drop, const bool isBrick, const bool isHidden) : GameObject(startPos)
{
	
	state = QuestionBlockState::Closed;
	const auto t = Textures::GetInstance()->Get(BLOCKS_OVERWORLD_TEX_ID);
	const auto sp = Sprites::GetInstance();
	const auto anims = Animations::GetInstance();


	// question block
	if (!isBrick && !anims->Contains(QUESTION_BLOCK_OVERWORLD_IDLE_ANIM_ID))
	{
		sp->Add(QUESTION_BLOCK_OVERWORLD_IDLE_SPRITE_1, 0, 0, 15, 15, t);
		sp->Add(QUESTION_BLOCK_OVERWORLD_IDLE_SPRITE_2, 16, 0, 31, 15, t);
		sp->Add(QUESTION_BLOCK_OVERWORLD_IDLE_SPRITE_3, 32, 0, 47, 15, t);

		// idle
		auto anim = new Animation(200);
		anim->Add(QUESTION_BLOCK_OVERWORLD_IDLE_SPRITE_1);
		anim->Add(QUESTION_BLOCK_OVERWORLD_IDLE_SPRITE_2);
		anim->Add(QUESTION_BLOCK_OVERWORLD_IDLE_SPRITE_3);
		anims->Add(QUESTION_BLOCK_OVERWORLD_IDLE_ANIM_ID, anim);
	}

	if (isBrick && !anims->Contains(BRICK_OVERWORLD_IDLE_ANIM_ID))
	{
		// brick
		sp->Add(BRICK_OVERWORLD_IDLE_SPRITE_1, 0, 16, 15, 31, t);

		auto anim = new Animation(0);
		anim->Add(BRICK_OVERWORLD_IDLE_SPRITE_1);
		anims->Add(BRICK_OVERWORLD_IDLE_ANIM_ID, anim);
	}

	if (!anims->Contains(BLOCK_OVERWORLD_BLOCKED_ANIM_ID))
	{
		// blocked
		sp->Add(BLOCK_OVERWORLD_BLOCKED_SPRITE_1, 48, 0, 63, 15, t);
		auto anim = new Animation(0);
		anim->Add(BLOCK_OVERWORLD_BLOCKED_SPRITE_1);
		anims->Add(BLOCK_OVERWORLD_BLOCKED_ANIM_ID, anim);
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


