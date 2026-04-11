#include "QuestionBlock.h"

#include "Animations.h"
#include "AssetIDs.h"
#include "AudioManager.h"
#include "Coin.h"
#include "Debug.h"
#include "Flower.h"
#include "Game.h"
#include "Goomba.h"
#include "Mushroom.h"
#include "Sprites.h"
#include "Texture.h"
#include "Textures.h"

void QuestionBlock::SetState(const QuestionBlockState newState)
{
	AudioManager::GetInstance()->PlaySFX(MARIO_BUMP);


	if (state == QuestionBlockState::Blocked)
		return;

	state = newState;
	if (newState == QuestionBlockState::Opened)
	{
		moveUpTimer.Start();
	}
}

void QuestionBlock::Render()
{
	
	const int animID = state == QuestionBlockState::Blocked ? 
	QUESTION_BLOCK_OVERWORLD_BLOCKED_ANIM_ID: 
	QUESTION_BLOCK_OVERWORLD_IDLE_ANIM_ID;


	const auto anim = Animations::GetInstance()->Get(animID);
	float renderX, renderY;

	Game::GetInstance()
	->GetCamera()
	->WorldToScreen(renderPosition.x, renderPosition.y, renderX, renderY);
	anim->Render(round(renderX), round(renderY), false, false);
}

void QuestionBlock::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (state == QuestionBlockState::Blocked)
		return;

	if (state != QuestionBlockState::Opened)
		return;

	if (!spawnInternalItem)
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

		if (drop == BlockDropType::Coin)
		{
			ctx->addObject(new Coin(
				Vector2Int(startPosition.x, startPosition.y),
				CoinState::CollectedFromQuestionBox)
			);
		}else if (drop == BlockDropType::JewDestroyer)
		{
			ctx->addObject(new Mushroom(position));
		}
		spawnInternalItem = true;
	}

	moveUpTimer.ProcessTimer(dt);
	if (!moveUpTimer.IsFinished())
	{
		renderPosition.y -= 125.0f * dt;



	}else
	{


		if (renderPosition.y < startPosition.y)
		{
			renderPosition.y += 9000.0f * dt * dt;

		}else
		{
			moveUpTimer.SetIdle();
			renderPosition.y = startPosition.y;
			state = QuestionBlockState::Blocked;
		}
	}
}

QuestionBlock::QuestionBlock(const Vector2Int startPos, const BlockDropType drop) : GameObject(startPos.x, startPos.y)
{
	spawnInternalItem = false;
	moveUpTimer = Timer(0.1f);
	this->drop = drop;
	state = QuestionBlockState::Closed;
	const auto t = Textures::GetInstance()->Get(QUESTION_BLOCK_OVERWORLD_TEX_ID);
	const auto sp = Sprites::GetInstance();
	const auto anims = Animations::GetInstance();

	sp->Add(QUESTION_BLOCK_OVERWORLD_IDLE_SPRITE_1, 0, 0, 15, 15, t);
	sp->Add(QUESTION_BLOCK_OVERWORLD_IDLE_SPRITE_2, 16, 0, 31, 15, t);
	sp->Add(QUESTION_BLOCK_OVERWORLD_IDLE_SPRITE_3, 32, 0, 47, 15, t);
	sp->Add(QUESTION_BLOCK_OVERWORLD_BLOCKED_SPRITE_1, 48, 0, 63, 15, t);


	// idle
	auto anim = new Animation(200);
	anim->Add(QUESTION_BLOCK_OVERWORLD_IDLE_SPRITE_1);
	anim->Add(QUESTION_BLOCK_OVERWORLD_IDLE_SPRITE_2);
	anim->Add(QUESTION_BLOCK_OVERWORLD_IDLE_SPRITE_3);
	anims->Add(QUESTION_BLOCK_OVERWORLD_IDLE_ANIM_ID, anim);

	// blocked
	anim = new Animation(0);
	anim->Add(QUESTION_BLOCK_OVERWORLD_BLOCKED_SPRITE_1);
	anims->Add(QUESTION_BLOCK_OVERWORLD_BLOCKED_ANIM_ID, anim);


	startPosition = position;
	renderPosition = position;

	bounceCheckBox = Rect::FromXYWH(startPos.x, startPos.y - 16, 16, 16);
}

Rect QuestionBlock::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 16, 16);
}


