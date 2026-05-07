#include "Bloopers.h"

#include "Animations.h"
#include "AssetIDs.h"
#include "Collision.h"
#include "Game.h"
#include "Mario.h"
#include "Sprites.h"
#include "Textures.h"

constexpr float BLOOPER_UP_SPEED = 45.0f;
constexpr float BLOOPER_DOWN_SPEED = 25.0f;
constexpr float BLOOPER_SIDE_SPEED = 20.0f;

Bloopers::Bloopers(const Vector2Int lowestLimit, const Vector2Int highestLimit) : GameObject(lowestLimit)
{
	this->lowestLimit = Vector2(lowestLimit);
	this->highestLimit = Vector2(highestLimit);
	movingUp = true;

	const auto t = Textures::GetInstance()->Get(BLOOPERS_TEX_ID);
	const auto anims = Animations::GetInstance();
	const auto sp = Sprites::GetInstance();

	if (!anims->Contains(WATER_BLOOPER_IDLE_ANIM))
	{
		auto anim = new Animation();
		sp->Add(WATER_BLOOPER_IDLE_SPRITE_1, 0, 0, 15, 23, t);
		anim->Add(WATER_BLOOPER_IDLE_SPRITE_1);
		anims->Add(WATER_BLOOPER_IDLE_ANIM, anim);
	}

	if (!anims->Contains(WATER_BLOOPER_SWIM_ANIM))
	{
		auto anim = new Animation();
		sp->Add(WATER_BLOOPER_SWIM_SPRITE_1, 16, 0, 31, 15, t);
		anim->Add(WATER_BLOOPER_SWIM_SPRITE_1);
		anims->Add(WATER_BLOOPER_SWIM_ANIM, anim);
	}
}

void Bloopers::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (movingUp)
	{
		velocity.y = -BLOOPER_UP_SPEED;
		velocity.x = ctx->mario->position.x < position.x ? -BLOOPER_SIDE_SPEED : BLOOPER_SIDE_SPEED;

		if (position.y <= highestLimit.y)
		{
			position.y = highestLimit.y;
			movingUp = false;
		}
	}
	else
	{
		velocity.y = BLOOPER_DOWN_SPEED;
		velocity.x = 0.0f;

		if (position.y >= lowestLimit.y)
		{
			position.y = lowestLimit.y;
			movingUp = true;
		}
	}

	Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);
}

void Bloopers::Render()
{
	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);
	
	int renderId = movingUp ? WATER_BLOOPER_SWIM_ANIM : WATER_BLOOPER_IDLE_ANIM;
	Animations::GetInstance()->Get(renderId)->Render(round(renderX), round(renderY), false, false);
}

void Bloopers::OnNoCollision(float dt)
{
	position += velocity * dt;
}

Rect Bloopers::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 16, movingUp ? 16 : 24);
}
