#include "Coin.h"

#include "Animations.h"
#include "AssetIDs.h"
#include "AudioManager.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"

void Coin::SetState(CoinState s)
{
	state = s;
	if (state == CoinState::Collected)
	{
		isDeleted = true;
	}
}

void Coin::Render()
{
	float renderX, renderY;

	Game::GetInstance()
	->GetCamera()
	->WorldToScreen(position.x, position.y, renderX, renderY);

	Animations::GetInstance()->Get(COIN_SPIN_ANIM_ID)
	->Render(round(renderX), round(renderY), false, false);
}

void Coin::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (state != CoinState::CollectedFromQuestionBox)
		return;

	moveUpTimer.ProcessTimer(dt);
	if (!moveUpTimer.IsFinished())
	{
		position.y -= 200 * dt;
	}else
	{
		moveUpTimer.SetIdle();
		isDeleted = true;
		state = CoinState::Collected;
	}
}

Coin::Coin(Vector2Int startPos, const CoinState state) : GameObject(startPos.x, startPos.y)
{
	moveUpTimer = Timer(0.3f);
	this->state = state;
	isCollidable = true;

	if (state== CoinState::CollectedFromQuestionBox)
	{
		moveUpTimer.Start();
		AudioManager::GetInstance()->PlaySFX(MARIO_COLLECT_COIN);
		isCollidable = false;
	}

	auto t = Textures::GetInstance()->Get(OVERWORLD_ITEMS_TEX_ID);
	auto sp = Sprites::GetInstance();
	auto anims = Animations::GetInstance();

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

Rect Coin::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 16, 16);
}
