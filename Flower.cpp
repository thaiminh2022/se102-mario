#include "Flower.h"

#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "AudioManager.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"

void Flower::SetState(CollectableItemState state)
{
	this->state = state;
	if (state == CollectableItemState::Collected)
	{
		isDeleted = true;
	}
}

Flower::Flower(Vector2 startPos) : GameObject(startPos.x, startPos.y)
{
	state = CollectableItemState::Emerging;
	preferPos = position;

	auto anims = Animations::GetInstance();

	if (!anims->Contains(FLOWER_IDLE_ANIM_ID))
	{
		preferPos.y -= 16;

		auto t = Textures::GetInstance()->Get(OVERWORLD_ITEMS_TEX_ID);
		auto sp = Sprites::GetInstance();


		sp->Add(FLOWER_IDLE_SPRITE_1, 0, 16, 15, 31, t);
		sp->Add(FLOWER_IDLE_SPRITE_2, 16, 16, 31, 31, t);
		sp->Add(FLOWER_IDLE_SPRITE_3, 32, 16, 47, 31, t);
		sp->Add(FLOWER_IDLE_SPRITE_4, 48, 16, 63, 31, t);


		auto anim = new Animation(300);
		anim->Add(FLOWER_IDLE_SPRITE_1);
		anim->Add(FLOWER_IDLE_SPRITE_2);
		anim->Add(FLOWER_IDLE_SPRITE_3);
		anim->Add(FLOWER_IDLE_SPRITE_4);
		anims->Add(FLOWER_IDLE_ANIM_ID, anim);
	}

	AudioManager::GetInstance()->PlaySFX(POWERUP_APPEARS);
}

void Flower::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (state == CollectableItemState::Emerging)
	{
		isCollidable = false;
		isBlocking = false;
		if (position.y > preferPos.y)
		{
			position.y -= 20 * dt;
		}else
		{
			state = CollectableItemState::Collectable;
			isCollidable = true;
			position.y = preferPos.y;
		}
	}
}

void Flower::Render()
{
	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);
	Animations::GetInstance()
	->Get(FLOWER_IDLE_ANIM_ID)
	->Render(round(renderX), round(renderY), false, false);
}

Rect Flower::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 16, 16);
}
