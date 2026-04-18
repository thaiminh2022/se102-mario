#include "FlagPole.h"

#include "Animations.h"
#include "AssetIDs.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"

FlagPole::FlagPole(Rect r, Vector2Int moveTo)
{
	state = FlagPoleState::Idle;
	auto anims = Animations::GetInstance();
	auto sp = Sprites::GetInstance();
	auto t = Textures::GetInstance()->Get(OVERWORLD_ITEMS_TEX_ID);

	if (!anims->Contains(FLAG_IDLE_ANIM_ID))
	{
		const auto anim = new Animation;
		sp->Add(FLAG_IDLE_SPRITE_1, 0, 64, 15, 79, t);
		anim->Add(FLAG_IDLE_SPRITE_1);
		anims->Add(FLAG_IDLE_ANIM_ID, anim);
	}
	position = Vector2Int(r.left, r.top);
	flagPosition = position + Vector2(-9, 0);

	zone = r;
	playerMoveTo = moveTo;
}

Rect FlagPole::GetBoundingBox()
{
	return zone;
}

void FlagPole::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (state != FlagPoleState::Move)
		return;
	constexpr float offset = -16;
	if (flagPosition.y < GetBoundingBox().bottom + offset)
	{
		constexpr float flagMoveSpeed = 150;
		flagPosition.y += flagMoveSpeed * dt;
	}else
	{
		flagPosition.y = GetBoundingBox().bottom + offset;
		state = FlagPoleState::Idle;
	}
}

void FlagPole::Render()
{
	float renderX, renderY;
	Game::GetInstance()
	->GetCamera()
	->WorldToScreen(flagPosition.x, flagPosition.y, renderX, renderY);

	Animations::GetInstance()
	->Get(FLAG_IDLE_ANIM_ID)
	->Render(round(renderX), round(renderY), false, false);
}

void FlagPole::SetFlagMove()
{
	state = FlagPoleState::Move;
}

Vector2 FlagPole::GetSnapPosition() const
{
	constexpr float offsetX = -7;
	return Vector2(flagPosition.x, position.y);
}

Vector2 FlagPole::GetMoveToPosition()
{
	return playerMoveTo;
}




