#include "CheepCheeps.h"

#include "Animations.h"
#include "AssetIDs.h"
#include "Collision.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"

CheepCheeps::CheepCheeps(const Vector2Int startPos, bool isRed) : GameObject(startPos)
{
	this->isRed = isRed;

	const auto t = Textures::GetInstance()->Get(CHEEPS_TEX_ID);
	const auto anims = Animations::GetInstance();
	const auto sp = Sprites::GetInstance();

	if (!anims->Contains(RED_CHEEP_ANIM))
	{
		auto anim = new Animation();
		sp->Add(RED_CHEEP_SPRITE_1, 0, 16, 15, 31, t);
		sp->Add(RED_CHEEP_SPRITE_2, 16, 16, 31, 31, t);

		anim->Add(RED_CHEEP_SPRITE_1);
		anim->Add(RED_CHEEP_SPRITE_2);
		anims->Add(RED_CHEEP_ANIM, anim);
	}

	if (!anims->Contains(WATER_CHEEP_ANIM))
	{
		auto anim = new Animation();
		sp->Add(WATER_CHEEP_SPRITE_1, 0, 0, 15, 15, t);
		sp->Add(WATER_CHEEP_SPRITE_2, 16, 0, 31, 15, t);


		anim->Add(WATER_CHEEP_SPRITE_1);
		anim->Add(WATER_CHEEP_SPRITE_2);
		anims->Add(WATER_CHEEP_ANIM, anim);
	}
}

void CheepCheeps::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	velocity.x = isRed ? -75.0f : -50.0f;

	Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);
}

void CheepCheeps::Render()
{
	const auto chosenAnimId = isRed ? RED_CHEEP_ANIM : WATER_CHEEP_ANIM;

	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);
	Animations::GetInstance()->Get(chosenAnimId)->Render(round(renderX), round(renderY), false, false);
}

void CheepCheeps::OnNoCollision(float dt)
{
	position += velocity * dt;
}

Rect CheepCheeps::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 16, 16);
}

