#include "PointPopup.h"

#include "Animation.h"
#include "Animations.h"
#include "Game.h"
#include "AssetIDs.h"
#include "Sprites.h"
#include "Textures.h"
#include "Debug.h"

void PointPopup::EnsureAssets(int scoreValue)
{
	auto anims = Animations::GetInstance();
	int id = GetAnimIdForScore(scoreValue);

	// 1. Efficiently exit if already loaded
	if (anims->Contains(id))
		return;

	auto texture = Textures::GetInstance()->Get(POINTS_TEX_ID);
	auto sprites = Sprites::GetInstance();

	// Create the animation ONLY when we are sure we need to add a new one
	Animation* anim = new Animation(0);

	switch (scoreValue)
	{
	case 100:
		sprites->Add(POINT_100_SPRITE_1, 0, 0, 15, 7, texture);
		anim->Add(POINT_100_SPRITE_1);
		break;
	case 200:
		sprites->Add(POINT_200_SPRITE_1, 0, 10, 15, 17, texture);
		anim->Add(POINT_200_SPRITE_1);
		break;
	case 400:
		sprites->Add(POINT_400_SPRITE_1, 0, 20, 15, 27, texture);
		anim->Add(POINT_400_SPRITE_1);
		break;
	case 500:
		sprites->Add(POINT_500_SPRITE_1, 0, 30, 15, 37, texture);
		anim->Add(POINT_500_SPRITE_1);
		break;
	case 800:
		sprites->Add(POINT_800_SPRITE_1, 0, 40, 15, 47, texture);
		anim->Add(POINT_800_SPRITE_1);
		break;
	case 1000:
		sprites->Add(POINT_1000_SPRITE_1, 18, 0, 33, 7, texture);
		anim->Add(POINT_1000_SPRITE_1);
		break;
	case 2000:
		sprites->Add(POINT_2000_SPRITE_1, 18, 10, 33, 17, texture);
		anim->Add(POINT_2000_SPRITE_1);
		break;
	case 4000:
		sprites->Add(POINT_4000_SPRITE_1, 18, 20, 33, 27, texture);
		anim->Add(POINT_4000_SPRITE_1);
		break;
	case 5000:
		sprites->Add(POINT_5000_SPRITE_1, 18, 30, 33, 37, texture);
		anim->Add(POINT_5000_SPRITE_1);
		break;
	case 8000:
		sprites->Add(POINT_8000_SPRITE_1, 18, 40, 33, 47, texture);
		anim->Add(POINT_8000_SPRITE_1);
		break;
	case 1:
		sprites->Add(ONEUP_SPRITE_1, 18, 50, 33, 56, texture);
		anim->Add(ONEUP_SPRITE_1);
		break;
	default:
		// Cleanup the unused animation object to prevent memory leaks
		delete anim;
		return;
	}
	anims->Add(id, anim);
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
	case 1:
		return ONEUP_ANIM_ID;
	default:
		return -1; // Invalid ID to indicate an error
	}
}

PointPopup::PointPopup(Vector2 pos, int scoreValue) : GameObject(pos)
{
	animId = GetAnimIdForScore(scoreValue);
	if (animId == -1)
	{
		DebugOut(L"[ERROR] Invalid score value %d for PointPopup\n", scoreValue);
		this->isDeleted = true;
		return;
	}
	EnsureAssets(scoreValue);
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
