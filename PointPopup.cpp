#include "PointPopup.h"

#include "Animation.h"
#include "Animations.h"
#include "Game.h"
#include "AssetIDs.h"
#include "Sprites.h"
#include "Textures.h"
#include "Debug.h"

void PointPopup::EnsureAssets()
{
	auto anims = Animations::GetInstance();
	if (anims->Contains(POINT_100_ANIM_ID)
		&& anims->Contains(POINT_200_ANIM_ID)
		&& anims->Contains(POINT_400_ANIM_ID)
		&& anims->Contains(POINT_500_ANIM_ID)
		&& anims->Contains(POINT_800_ANIM_ID)
		&& anims->Contains(POINT_1000_ANIM_ID))
	{
		return;
	}

	auto texture = Textures::GetInstance()->Get(POINTS_TEX_ID);
	auto sprites = Sprites::GetInstance();

	sprites->Add(POINT_100_SPRITE_1, 0, 0, 15, 7, texture);
	sprites->Add(POINT_200_SPRITE_1, 0, 10, 15, 17, texture);
	sprites->Add(POINT_400_SPRITE_1, 0, 20, 15, 27, texture);
	sprites->Add(POINT_500_SPRITE_1, 0, 30, 15, 37, texture);
	sprites->Add(POINT_800_SPRITE_1, 0, 40, 15, 47, texture);
	sprites->Add(POINT_1000_SPRITE_1, 18, 0, 33, 7, texture);
	sprites->Add(POINT_2000_SPRITE_1, 18, 10, 33, 17, texture);
	sprites->Add(POINT_4000_SPRITE_1, 18, 20, 33, 27, texture);
	sprites->Add(POINT_5000_SPRITE_1, 18, 30, 33, 37, texture);
	sprites->Add(POINT_8000_SPRITE_1, 18, 40, 33, 47, texture);

	auto anim100 = new Animation(0);
	anim100->Add(POINT_100_SPRITE_1);
	anims->Add(POINT_100_ANIM_ID, anim100);

	auto anim200 = new Animation(0);
	anim200->Add(POINT_200_SPRITE_1);
	anims->Add(POINT_200_ANIM_ID, anim200);

	auto anim400 = new Animation(0);
	anim400->Add(POINT_400_SPRITE_1);
	anims->Add(POINT_400_ANIM_ID, anim400);

	auto anim500 = new Animation(0);
	anim500->Add(POINT_500_SPRITE_1);
	anims->Add(POINT_500_ANIM_ID, anim500);

	auto anim800 = new Animation(0);
	anim800->Add(POINT_800_SPRITE_1);
	anims->Add(POINT_800_ANIM_ID, anim800);

	auto anim1000 = new Animation(0);
	anim1000->Add(POINT_1000_SPRITE_1);
	anims->Add(POINT_1000_ANIM_ID, anim1000);

	auto anim2000 = new Animation(0);
	anim2000->Add(POINT_2000_SPRITE_1);
	anims->Add(POINT_2000_ANIM_ID, anim2000);

	auto anim4000 = new Animation(0);
	anim4000->Add(POINT_4000_SPRITE_1);
	anims->Add(POINT_4000_ANIM_ID, anim4000);

	auto anim5000 = new Animation(0);
	anim5000->Add(POINT_5000_SPRITE_1);
	anims->Add(POINT_5000_ANIM_ID, anim5000);

	auto anim8000 = new Animation(0);
	anim8000->Add(POINT_8000_SPRITE_1);
	anims->Add(POINT_8000_ANIM_ID, anim8000);
}

int PointPopup::GetAnimIdForScore(const int scoreValue)
{
	switch (scoreValue)
	{
	case 100:
		return POINT_100_ANIM_ID;
	case 200:
		return POINT_200_ANIM_ID;
	case 400:
		return POINT_400_ANIM_ID;
	case 500:
		return POINT_500_ANIM_ID;
	case 800:
		return POINT_800_ANIM_ID;
	case 1000:
		return POINT_1000_ANIM_ID;
	case 2000:
		return POINT_2000_ANIM_ID;
	case 4000:
		return POINT_4000_ANIM_ID;
	case 5000:
		return POINT_5000_ANIM_ID;
	case 8000:
		return POINT_8000_ANIM_ID;
	default:
		return POINT_100_ANIM_ID;
	}
}

PointPopup::PointPopup(Vector2 pos, int scoreValue) : GameObject(pos)
{
	EnsureAssets();
	animId = GetAnimIdForScore(scoreValue);
	lifeTimer = Timer(0.6f);
	lifeTimer.Start();
	riseSpeed = 30.0f;
	isCollidable = false;
	isBlocking = false;
}

void PointPopup::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	position.y -= riseSpeed * dt;
	lifeTimer.ProcessTimer(dt);

	if (lifeTimer.IsFinished())
	{
		isDeleted = true;
	}
}

void PointPopup::Render()
{
	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);

	Animation* anim = Animations::GetInstance()->Get(animId);
	if (anim != nullptr)
	{
		anim->Render(round(renderX), round(renderY), false, false);
	}
	else
	{
		// Optional: Print a warning to the console so you know an ID is missing
		DebugOutTitle(L"[WARNING] PointPopup tried to render missing animId: %d\n", animId);
	}
}

Rect PointPopup::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 0, 0);
}
