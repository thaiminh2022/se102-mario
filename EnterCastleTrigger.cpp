#include "EnterCastleTrigger.h"

#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "AudioManager.h"
#include "Game.h"
#include "Helper.h"
#include "Mario.h"
#include "Sprites.h"
#include "Textures.h"

void EnterCastleTrigger::SetNewFireworkPosition()
{
	const auto randomIndex = rand() % data.fireworkPositions.size();
	fireworkPosition = data.fireworkPositions[randomIndex];
}

EnterCastleTrigger::EnterCastleTrigger(const EnterCastleTriggerData& data, BiomeType biome)
{
	this->data = data;
	isTriggered = false;

	// firework
	const auto anims = Animations::GetInstance();
	const auto sprites = Sprites::GetInstance();
	timePerFireworkTimer = Timer(0.5);

	if (!anims->Contains(FIREBALL_COLLIDE_ANIM_ID))
	{
		const auto tex = Textures::GetInstance()->Get(FIREBALL_TEX_ID);
		// COLLISION
		sprites->Add(FIREBALL_COLLIDE_SPRITE_1, 40, 0, 55, 15, tex);
		sprites->Add(FIREBALL_COLLIDE_SPRITE_2, 58, 0, 73, 15, tex);
		sprites->Add(FIREBALL_COLLIDE_SPRITE_3, 76, 0, 91, 15, tex);
		auto anim = new Animation(150);
		anim->Add(FIREBALL_COLLIDE_SPRITE_1);
		anim->Add(FIREBALL_COLLIDE_SPRITE_2);
		anim->Add(FIREBALL_COLLIDE_SPRITE_3);
		anims->Add(FIREBALL_COLLIDE_ANIM_ID, anim);
	}

	// castle flag
	const auto& itemTex = Textures::GetInstance()->Get(ChooseItemsId(biome));
	sprites->Add(CASTLE_FLAG_SPRITE_1, 16, 64, 31, 79, itemTex);
	if (data.flagMoveTo.has_value())
	{
		flagPosition = data.flagMoveTo.value() + Vector2Int(0, 64);
		flagFinishMoving = false;
	}
}

void EnterCastleTrigger::Update(const float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (!isTriggered)
	{
		if (ctx->mario->GetBoundingBox().IsColliding(GetBoundingBox()))
		{
			isTriggered = true;
			timePerFireworkTimer.Start();
		}
	}

	if (!data.fireworkPositions.empty())
	{
		timePerFireworkTimer.ProcessTimer(dt);
		if (timePerFireworkTimer.IsFinished())
		{
			SetNewFireworkPosition();
			timePerFireworkTimer.Start();
			AudioManager::GetInstance()->PlaySFX(FIREWORK);
		}
	}

	if (data.flagMoveTo.has_value() && !flagFinishMoving)
	{
		const auto& flagMoveTo = data.flagMoveTo.value();
		if (flagPosition.y > flagMoveTo.y)
		{
			constexpr float speed = 15.0f;
			flagPosition.y -= speed * dt;

			if (flagPosition.y < flagMoveTo.y)
			{
				flagPosition.y = flagMoveTo.y;
				flagFinishMoving = true;
			}
		}
	}

}

void EnterCastleTrigger::Render()
{
	if (!isTriggered)
		return;

	if (!data.fireworkPositions.empty())
	{
		float renderX, renderY;
		Game::GetInstance()->GetCamera()->WorldToScreen(fireworkPosition.x, fireworkPosition.y, renderX, renderY);
		Animations::GetInstance()
			->Get(FIREBALL_COLLIDE_ANIM_ID)
			->Render(renderX, renderY, false, false);
	}

	if (data.flagMoveTo.has_value())
	{
		float renderX, renderY;
		Game::GetInstance()->GetCamera()->WorldToScreen(flagPosition.x, flagPosition.y, renderX, renderY);
		Sprites::GetInstance()
			->Get(CASTLE_FLAG_SPRITE_1)
			->Draw(round(renderX), round(renderY), false, false);
	}

}

int EnterCastleTrigger::GetRenderIndex()
{
	return -10;
}

bool EnterCastleTrigger::IsCollidable()
{
	return false;
}

Rect EnterCastleTrigger::GetBoundingBox()
{
	return data.zone;
}
