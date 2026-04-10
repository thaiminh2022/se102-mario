#include "Mushroom.h"

#include "Animations.h"
#include "AssetIDs.h"
#include "AudioManager.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"

Mushroom::Mushroom(const Vector2 startPos) : GameObject(startPos.x, startPos.y)
{

	state = MushroomState::Emerging;
	preferPosition = startPos;
	preferPosition.y -= 16;


	auto t = Textures::GetInstance()->Get(OVERWORLD_ITEMS_TEX_ID);
	auto sp = Sprites::GetInstance();
	auto anims = Animations::GetInstance();

	sp->Add(MUSHROOM_SPRITE_1, 0, 48, 15, 63, t);

	auto anim = new Animation;
	anim->Add(MUSHROOM_SPRITE_1);
	anims->Add(MUSHROOM_ANIM_ID, anim);
	AudioManager::GetInstance()->PlaySFX(POWERUP_APPEARS);
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
	if (state == MushroomState::Emerging)
	{
		isCollidable = false;

		if (position.y > preferPosition.y)
		{
			position.y -= 50 * dt;
		}else
		{
			position.y = preferPosition.y;
			state = MushroomState::Moving;
			isCollidable = true;
		}
	}else if (state == MushroomState::Moving)
	{
		// default to move left
		velocity.x = 100.0f;
		velocity.y += 900.0f * dt;
		Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);
	}
}

void Mushroom::OnNoCollision(float dt)
{
	position += velocity * dt;
}

Rect Mushroom::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 16, 16);
}
