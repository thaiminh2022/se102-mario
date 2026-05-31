#include "InWaterTrigger.h"

#include "Mario.h"

InWaterTrigger::InWaterTrigger(const Rect zone, const bool inWater)
{
	this->zone = zone;
	this->inWater = inWater;
	triggered = false;
}

Rect InWaterTrigger::GetBoundingBox()
{
	return zone;
}

void InWaterTrigger::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (!triggered && ctx->mario->GetBoundingBox().IsColliding(zone))
	{
		ctx->mario->SetIsInWater(inWater);
		isDeleted = true;
		triggered = true;
	}
}
