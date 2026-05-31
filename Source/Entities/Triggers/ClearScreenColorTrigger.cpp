#include "ClearScreenColorTrigger.h"

#include "Game.h"
#include "Mario.h"

ClearScreenColorTrigger::ClearScreenColorTrigger(const Rect z, const Color c)
{
	zone = z;
	color = c;
	triggered = false;
}

Rect ClearScreenColorTrigger::GetBoundingBox()
{
	return zone;
}

void ClearScreenColorTrigger::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (!triggered && ctx->mario->GetBoundingBox().IsColliding(GetBoundingBox()))
	{
		Game::GetInstance()->SetBackgroundColor(color);
		triggered = true;
		isDeleted = true;
	}
}
