#include "Star.h"

#include "Animations.h"
#include "AssetIDs.h"
#include "AudioManager.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"

void Star::SetState(CollectableItemState state)
{
	this->state = state;
	if (state == CollectableItemState::Collected)
	{
		isDeleted = true;
	}
}

Star::Star(Vector2 startPos) : GameObject(startPos.x, startPos.y)
{
	state = CollectableItemState::Emerging;
	preferPos = position;
	preferPos.y -= 16;

	auto anims = Animations::GetInstance();

	if (!anims->Contains(STAR_IDLE_ANIM_ID))
	{
		auto t = Textures::GetInstance()->Get(OVERWORLD_ITEMS_TEX_ID);
		auto sp = Sprites::GetInstance();

		sp->Add(STAR_IDLE_SPRITE_1, 0, 32, 15, 47, t);
		sp->Add(STAR_IDLE_SPRITE_2, 16, 32, 31, 47, t);
		sp->Add(STAR_IDLE_SPRITE_3, 32, 32, 47, 47, t);
		sp->Add(STAR_IDLE_SPRITE_4, 48, 32, 63, 47, t);

		auto anim = new Animation(300);
		anim->Add(STAR_IDLE_SPRITE_1);
		anim->Add(STAR_IDLE_SPRITE_2);
		anim->Add(STAR_IDLE_SPRITE_3);
		anim->Add(STAR_IDLE_SPRITE_4);

		anims->Add(STAR_IDLE_ANIM_ID, anim);
	}


	AudioManager::GetInstance()->PlaySFX(POWERUP_APPEARS);
}

void Star::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (state == CollectableItemState::Emerging)
	{
		isCollidable = false;
		isBlocking = false;
		if (position.y > preferPos.y)
		{
			position.y -= 20 * dt;
		}
		else
		{
			state = CollectableItemState::Collectable;
			isCollidable = true;
			position.y = preferPos.y;
		}
	}
}

void Star::Render()
{
	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);
	Animations::GetInstance()
		->Get(FLOWER_IDLE_ANIM_ID)
		->Render(round(renderX), round(renderY), false, false);
}

Rect Star::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 16, 16);
}
