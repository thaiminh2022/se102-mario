#include "Mushroom.h"

#include "Animations.h"
#include "AssetIDs.h"
#include "AudioManager.h"
#include "Collision.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"

void Mushroom::SetState(CollectableItemState newState)
{
	state = newState;
	if (newState == CollectableItemState::Collected)
	{
		isDeleted = true;
	}
}

Mushroom::Mushroom(const Vector2 startPos) : GameObject(startPos.x, startPos.y)
{
	moveLeft = false;
	state = CollectableItemState::Emerging;
	preferPosition = startPos;
	preferPosition.y -= 16;

	auto anims = Animations::GetInstance();

	if (!anims->Contains(MUSHROOM_ANIM_ID))
	{
		auto t = Textures::GetInstance()->Get(OVERWORLD_ITEMS_TEX_ID);
		auto sp = Sprites::GetInstance();
		sp->Add(MUSHROOM_SPRITE_1, 0, 48, 15, 63, t);
		auto anim = new Animation;
		anim->Add(MUSHROOM_SPRITE_1);
		anims->Add(MUSHROOM_ANIM_ID, anim);
	}

	AudioManager::GetInstance()->PlaySFX(POWERUP_APPEARS);
}

void Mushroom::SetMoveDirX(int newDirX)
{
	if (newDirX > 0)
	{
		moveLeft = false;
	}else
	{
		moveLeft = true;
	}
	velocity.y = -150.0;
}


void Mushroom::Render()
{
	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);
	Animations::GetInstance()->Get(MUSHROOM_ANIM_ID)
	->Render(round(renderX), round(renderY), false, false);
}

void Mushroom::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (state == CollectableItemState::Emerging)
	{
		isCollidable = false;

		if (position.y > preferPosition.y)
		{
			position.y -= 50 * dt;
		}else
		{
			position.y = preferPosition.y;
			state = CollectableItemState::Collectable;
			isCollidable = true;
		}
		return;
	}
	if (state == CollectableItemState::Collectable)
	{
		// default to move left
		velocity.x = moveLeft ? -20.0f : 20.0f;
		velocity.y += 900.0f * dt;
	}
	Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);
}


void Mushroom::OnCollisionWith(CollisionEvent* event)
{
	if (event->IsTileCollision()
		&& event->otherTile->IsBlocking() && event->normalizedDir.x != 0)
	{
		if (event->normalizedDir.x > 0)
		{
			moveLeft = false;
		}
		else if (event->normalizedDir.x < 0)
		{
			moveLeft = true;
		}
	}

}

Rect Mushroom::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 16, 16);
}
