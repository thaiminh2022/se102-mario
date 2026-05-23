#include "Bridge.h"
#include "Game.h"
#include "Debug.h"
#include "Color.h"

#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "AudioManager.h"

Bridge:: Bridge(const BridgeData &bridgeData)
{
	this->bridgeData = bridgeData;
	this->state = BridgeState::Normal;
	position = Vector2(bridgeData.zone.left, bridgeData.zone.top);
	disappearTimer = Timer(0.05f);

	auto t = Textures::GetInstance()->Get(BLOCKS_CASTLE_TEX_ID); 
	auto s = Sprites::GetInstance();
	auto a = Animations::GetInstance();

	s->Add(BRIDGE_SPRITE_1, 80, 0, 95, 15, t);
	if (!a->Contains(BRIDGE_ANIM_ID))
	{
		Animation* ani = new Animation(100);
		ani->Add(BRIDGE_SPRITE_1);
		a->Add(BRIDGE_ANIM_ID, ani);
	}


}

void Bridge::SetState(BridgeState newState)
{
	state = newState;
	switch (state)
	{
		case BridgeState::Normal:
			isCollidable = true;
			isDeleted = false;
			break;
		case BridgeState::Disappeared:
			isCollidable = false;
			isDeleted = true;
			break;
	}
}

Rect Bridge::GetBoundingBox()
{
	return bridgeData.zone;
}

void Bridge::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (state == BridgeState::Disappearing)
	{
		if (!disappearTimer.IsTicking())
		{
			disappearTimer.Start();
		}
		disappearTimer.ProcessTimer(dt);
		if (disappearTimer.IsFinished())
		{
			bridgeData.zone.right -= 16;
			AudioManager::GetInstance()->PlaySFX(BREAK_BLOCK);
			if (bridgeData.zone.right <= bridgeData.zone.left)
			{
				bridgeData.zone.right = bridgeData.zone.left;
				SetState(BridgeState::Disappeared);
			}
		}
	}
}

void Bridge::Render()
{
	if (state == BridgeState::Disappeared )
		return;
	float renderX, renderY;
	Game::GetInstance()
		->GetCamera()
		->WorldToScreen(bridgeData.zone.left, bridgeData.zone.top, renderX, renderY);
		int renderWidth = bridgeData.zone.GetWidth();
		int offsetX = 0;
		while (renderWidth >16)
		{
			offsetX = bridgeData.zone.GetWidth() - renderWidth;
			Animations::GetInstance()
				->Get(BRIDGE_ANIM_ID)
				->Render(renderX + offsetX, renderY, 0, 0);
			renderWidth -= 16; 
		}
}

AxeBridge::AxeBridge(const Vector2Int& axePosition, Bridge* source)
{
	this->position = Vector2(axePosition);
	this->source = source;
	auto s = Sprites::GetInstance();
	auto a = Animations::GetInstance();
	auto t = Textures::GetInstance()->Get(CASTLE_ITEMS_TEX_ID);
	s->Add(AXE_SPRITE_1, 16, 48, 31, 63, t);
	if (!a->Contains(AXE_ANIM_ID))
	{
		Animation* ani = new Animation(100);
		ani->Add(AXE_SPRITE_1);
		a->Add(AXE_ANIM_ID, ani);
	}
	isCollidable = true;
	isDeleted = false;
}
void AxeBridge::SolveCollisionWithMario()
{
	this->isDeleted = true;
	source->SetState(BridgeState::Disappearing);
}
void AxeBridge::Render()
{
	float renderX, renderY;
	Game::GetInstance()
		->GetCamera()
		->WorldToScreen(position.x, position.y, renderX, renderY);
	Animations::GetInstance()
		->Get(AXE_ANIM_ID)
		->Render(renderX, renderY, 0, 0);
}
