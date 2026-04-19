#include "PointPopup.h"

#include "Animation.h"
#include "Animations.h"
#include "Game.h"
#include "AssetIDs.h"
#include "Sprites.h"
#include "Textures.h"

void PointPopup::EnsureAssets()
{
	auto anims = Animations::GetInstance();
	if (anims->Contains(POINT_100_ANIM_ID)
		&& anims->Contains(POINT_200_ANIM_ID)
		&& anims->Contains(POINT_1000_ANIM_ID))
	{
		return;
	}

	auto texture = Textures::GetInstance()->Get(POINTS_TEX_ID);
	auto sprites = Sprites::GetInstance();

	sprites->Add(POINT_100_SPRITE_1, 0, 0, 15, 7, texture);
	sprites->Add(POINT_200_SPRITE_1, 0, 10, 15, 17, texture);
	sprites->Add(POINT_1000_SPRITE_1, 18, 0, 33, 7, texture);

	auto anim100 = new Animation(0);
	anim100->Add(POINT_100_SPRITE_1);
	anims->Add(POINT_100_ANIM_ID, anim100);

	auto anim200 = new Animation(0);
	anim200->Add(POINT_200_SPRITE_1);
	anims->Add(POINT_200_ANIM_ID, anim200);

	auto anim1000 = new Animation(0);
	anim1000->Add(POINT_1000_SPRITE_1);
	anims->Add(POINT_1000_ANIM_ID, anim1000);
}

int PointPopup::GetAnimIdForScore(const int scoreValue)
{
	switch (scoreValue)
	{
	case 100:
		return POINT_100_ANIM_ID;
	case 200:
		return POINT_200_ANIM_ID;
	case 1000:
		return POINT_1000_ANIM_ID;
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

	Animations::GetInstance()->Get(animId)->Render(round(renderX), round(renderY), false, false);
}

Rect PointPopup::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 0, 0);
}
