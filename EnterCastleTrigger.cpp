#include "EnterCastleTrigger.h"

#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "Game.h"
#include "Mario.h"
#include "Sprites.h"

EnterCastleTrigger::EnterCastleTrigger(const EnterCastleTriggerData& data)
{
	this->data = data;
	isTriggered = false;

	const auto anims = Animations::GetInstance();
	const auto sprites = Sprites::GetInstance();
	if (!anims->Contains(FIREBALL_COLLIDE_ANIM_ID))
	{
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
}

void EnterCastleTrigger::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (!isTriggered)
	{
		if (ctx->mario->GetBoundingBox().IsColliding(GetBoundingBox()))
		{
			isTriggered = true;
		}
	}
}

void EnterCastleTrigger::Render()
{
	if (!isTriggered)
		return;

	for (const auto& fwPos : data.fireworkPositions)
	{
		float renderX, renderY;
		Game::GetInstance()->GetCamera()->WorldToScreen(fwPos.x, fwPos.y, renderX, renderY);
	
		Animations::GetInstance()
		->Get(FIREBALL_COLLIDE_ANIM_ID)
		->Render(renderX, renderY, false, false);
	}
}

int EnterCastleTrigger::GetRenderIndex()
{
	return 1;
}

bool EnterCastleTrigger::IsCollidable()
{
	return false;
}

Rect EnterCastleTrigger::GetBoundingBox()
{
	return data.zone;
}
