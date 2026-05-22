#include "MarioJetPack.h"

#include <algorithm>

#include "AssetIDs.h"
#include "FontManager.h"
#include "Game.h"
#include "Helper.h"
#include "Mario.h"
#include "Textures.h"
#include <format>

const float PMETER_MAX = 100.0f;
const float PMETER_CHARGE_RATE = 65.0f;
const float PMETER_DECAY_RATE = 55.0f;
const float PMETER_FLIGHT_DRAIN_RATE = 18.0f;

Rect MarioJetPack::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 16, 16);
}

MarioJetPack::MarioJetPack(Vector2Int startPos) : GameObject(startPos)
{
	auto anims = Animations::GetInstance();
	state = MarioJetPackState::Idle;

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

void MarioJetPack::UpdateMeter(float dt, bool canCharge)
{
	if (state != MarioJetPackState::OnMario)
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

	pMeter -= PMETER_DECAY_RATE * dt;
	pMeter = std::clamp(pMeter, 0.0f, PMETER_MAX);
	if (pMeter <= 0.0f)
	{
		readyToFly = false;
	}
}

void MarioJetPack::DrainFlight(float dt)
{
	if (state != MarioJetPackState::OnMario || !readyToFly)
		return;

	pMeter -= PMETER_FLIGHT_DRAIN_RATE * dt;
	pMeter = std::clamp(pMeter, 0.0f, PMETER_MAX);
	if (pMeter <= 0.0f)
	{
		readyToFly = false;
	}
}

void MarioJetPack::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (state == MarioJetPackState::Removed)
	{
		isDeleted = true;
	}

}
void MarioJetPack::SetState(MarioJetPackState s)
{
	state = s;
	isCollidable = state == MarioJetPackState::Idle;
}

void MarioJetPack::Render()
{
	if (state == MarioJetPackState::Idle)
	{
		float  renderX, renderY;
		Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);
		Animations::GetInstance()->Get(CROWN_IDLE_ANIM_ID)->Render(round(renderX), round(renderY), false, false);
		return;
	}


	if (state != MarioJetPackState::OnMario)
		return;

	// draw p-meter
	const auto drawY = Game::GetInstance()->GetBackBufferHeight() - 20;
	const auto str = std::format(L"P: {}%", static_cast<int>(std::round(pMeter)));
	
	FontManager::GetInstance()->Draw(STATS_FONT, Vector2Int(0, drawY), str.c_str() , Colors::WHITE);
}

void MarioJetPack::RenderCrownAt(const Vector2& marioPosition) const
{
	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(marioPosition.x, marioPosition.y - 12.0f, renderX, renderY);
	Animations::GetInstance()->Get(CROWN_WEAR_ANIM_ID)->Render(round(renderX), round(renderY), false, false);
}


bool MarioJetPack::IsBlocking()
{
	return false;
}
