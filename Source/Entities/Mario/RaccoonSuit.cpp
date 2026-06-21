#include "SuperLeaf.h"

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

	if (!anims->Contains(CROWN_IDLE_ANIM_ID))
	{
		auto t = Textures::GetInstance()->Get(CROWN_TEX_ID);
		auto sp = Sprites::GetInstance();
		sp->Add(CROWN_BIG_SPRITE, 0, 0, 15, 15, t);
		sp->Add(CROWN_SMALL_SPRITE, 16, 0, 31, 15, t);

		auto worldCrownAnim = new Animation;
		worldCrownAnim->Add(CROWN_BIG_SPRITE, 180);
		anims->Add(CROWN_IDLE_ANIM_ID, worldCrownAnim);

		auto wearCrownAnim = new Animation;
		wearCrownAnim->Add(CROWN_SMALL_SPRITE, 180);
		anims->Add(CROWN_WEAR_ANIM_ID, wearCrownAnim);
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
		Animations::GetInstance()->Get(CROWN_IDLE_ANIM_ID)->Render(round(renderX), round(renderY), false, false);
		return;
	}


	if (state != RaccoonSuitState::OnMario)
		return;

	// draw p-meter
	const auto drawY = Game::GetInstance()->GetBackBufferHeight() - 20;
	const auto str = std::format(L"P: {}%", static_cast<int>(std::round(pMeter)));
	
	FontManager::GetInstance()->Draw(STATS_FONT, Vector2Int(0, drawY), str.c_str() , Colors::WHITE);
}

void RaccoonSuit::RenderCrownAt(const Vector2& marioPosition) const
{
	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(marioPosition.x, marioPosition.y - 12.0f, renderX, renderY);
	Animations::GetInstance()->Get(CROWN_WEAR_ANIM_ID)->Render(round(renderX), round(renderY), false, false);
}

bool RaccoonSuit::IsBlocking()
{
	return false;
}
