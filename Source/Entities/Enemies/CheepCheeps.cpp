#include "CheepCheeps.h"

#include "Animations.h"
#include "AssetIDs.h"
#include "Collision.h"
#include "Game.h"
#include "Helper.h"
#include "Sprites.h"
#include "Textures.h"

CheepCheeps::CheepCheeps(Vector2Int startPos, BiomeType biome, bool isRed) : GameObject(startPos)
{
	this->isRed = isRed;
	deadTimer = Timer(2);

	const auto t = Textures::GetInstance()->Get(ChooseEnemyId(biome));
	const auto anims = Animations::GetInstance();
	const auto sp = Sprites::GetInstance();


	if (!anims->Contains(RED_CHEEP_ANIM))
	{
		auto anim = new Animation();
		sp->Add(RED_CHEEP_SPRITE_1, 0, 64, 15, 79, t);
		sp->Add(RED_CHEEP_SPRITE_2, 16, 64, 31, 79, t);

		anim->Add(RED_CHEEP_SPRITE_1);
		anim->Add(RED_CHEEP_SPRITE_2);
		anims->Add(RED_CHEEP_ANIM, anim);
	}

	if (!anims->Contains(WATER_CHEEP_ANIM))
	{
		auto anim = new Animation();
		sp->Add(WATER_CHEEP_SPRITE_1, 0, 48, 15, 63, t);
		sp->Add(WATER_CHEEP_SPRITE_2, 16, 48, 31, 63, t);


		anim->Add(WATER_CHEEP_SPRITE_1);
		anim->Add(WATER_CHEEP_SPRITE_2);
		anims->Add(WATER_CHEEP_ANIM, anim);
	}
}

void CheepCheeps::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (state == CheepCheepsState::Dead)
	{
		velocity.y = 250.0f;
		deadTimer.ProcessTimer(dt);
		if (deadTimer.IsFinished())
		{
			isDeleted = false;
			isCollidable = false;
			deadTimer.SetIdle();
		}

	}else
	{
		velocity.x = isRed ? -75.0f : -50.0f;
	}

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

