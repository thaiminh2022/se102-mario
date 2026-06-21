#include "RaccoonSuit.h"

#include <algorithm>

#include "AssetIDs.h"
#include "FontManager.h"
#include "Game.h"
#include "Mario.h"
#include "Textures.h"
#include <format>

#include "Animations.h"
#include "Sprites.h"



Rect RaccoonSuit::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 16, 16);
}

RaccoonSuit::RaccoonSuit(Vector2Int startPos) : GameObject(startPos)
{
	auto anims = Animations::GetInstance();
	state = RaccoonSuitState::Idle;

	if (!anims->Contains(SUPERLEAF_IDLE_ANIM_ID))
	{
		auto t = Textures::GetInstance()->Get(OVERWORLD_ITEMS_TEX_ID);
		auto sp = Sprites::GetInstance();
		sp->Add(SUPERLEAF_IDLE_SPRITE_1, 16, 48, 31, 63, t);

		auto anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
		anim->Add(SUPERLEAF_IDLE_SPRITE_1);
		anims->Add(SUPERLEAF_IDLE_ANIM_ID, anim);
	}
}

void RaccoonSuit::UpdateMeter(float dt, bool canCharge)
{
	if (state != RaccoonSuitState::OnMario)
		return;

	if (canCharge)
	{
		pMeter += PMETER_CHARGE_RATE * dt;
		pMeter = std::clamp(pMeter, 0.0f, PMETER_MAX);
		if (pMeter >= PMETER_MAX)
		{
			readyToFly = true;
		}
		return;
	}
	//lower the p-meter when not charging
	pMeter -= PMETER_DECAY_RATE * dt;
	pMeter = std::clamp(pMeter, 0.0f, PMETER_MAX);
	if (pMeter < PMETER_MAX)
	{
		readyToFly = false;
	}
}

void RaccoonSuit::DrainFlight(float dt)
{
	//used to lower the p-meter when FLYING
	if (state != RaccoonSuitState::OnMario || !readyToFly)
		return;

	pMeter -= PMETER_FLIGHT_DRAIN_RATE * dt;
	pMeter = std::clamp(pMeter, 0.0f, PMETER_MAX);
	if (pMeter < PMETER_MAX)
	{
		readyToFly = false;
	}
}

void RaccoonSuit::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (state == RaccoonSuitState::Removed)
	{
		isDeleted = true;
	}
}

void RaccoonSuit::SetState(RaccoonSuitState s)
{
	state = s;
	isCollidable = state == RaccoonSuitState::Idle;
}

void RaccoonSuit::Render()
{
	if (state == RaccoonSuitState::Idle)
	{
		float  renderX, renderY;
		Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);
		Animations::GetInstance()->Get(SUPERLEAF_IDLE_ANIM_ID)->Render(round(renderX), round(renderY), false, false);
		return;
	}


	if (state != RaccoonSuitState::OnMario)
		return;

	// draw p-meter
	const auto drawY = Game::GetInstance()->GetBackBufferHeight() - 20;
	const auto str = std::format(L"P: {}%", static_cast<int>(std::round(pMeter)));
	
	FontManager::GetInstance()->Draw(STATS_FONT, Vector2Int(0, drawY), str.c_str() , Colors::WHITE);
}

bool RaccoonSuit::IsBlocking()
{
	return false;
}