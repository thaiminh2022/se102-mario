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

	if (!anims->Contains(COIN_SPIN_ANIM_ID))
	{
		auto t = Textures::GetInstance()->Get(ChooseItemsId(BiomeType::Overworld));
		auto sp = Sprites::GetInstance();
		sp->Add(COIN_SPIN_SPRITE_1, 0, 0, 15, 15, t);
		sp->Add(COIN_SPIN_SPRITE_2, 16, 0, 31, 15, t);
		sp->Add(COIN_SPIN_SPRITE_3, 32, 0, 47, 15, t);
		sp->Add(COIN_SPIN_SPRITE_4, 48, 0, 63, 15, t);

		auto anim = new Animation;
		anim->Add(COIN_SPIN_SPRITE_1);
		anim->Add(COIN_SPIN_SPRITE_2);
		anim->Add(COIN_SPIN_SPRITE_3);
		anim->Add(COIN_SPIN_SPRITE_4);

		anims->Add(COIN_SPIN_ANIM_ID, anim);
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
		Animations::GetInstance()->Get(COIN_SPIN_ANIM_ID)->Render(round(renderX), round(renderY), false, false);
		return;
	}


	if (state != MarioJetPackState::OnMario)
		return;

	// draw p-meter
	const auto drawY = Game::GetInstance()->GetBackBufferHeight() - 20;
	const auto str = std::format(L"P: {}%", static_cast<int>(std::round(pMeter)));
	
	FontManager::GetInstance()->Draw(STATS_FONT, Vector2Int(0, drawY), str.c_str() , Colors::WHITE);
}


bool MarioJetPack::IsBlocking()
{
	return false;
}
