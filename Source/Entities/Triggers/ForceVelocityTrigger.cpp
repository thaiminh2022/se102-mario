#include "ForceVelocityTrigger.h"

#include "Mario.h"

ForceVelocityTrigger::ForceVelocityTrigger(ForceVelocityTriggerData data)
{
	this->data = data;
	position = Vector2Int(data.zone.left, data.zone.top);
}

void ForceVelocityTrigger::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (triggered)
	{
		isDeleted = true;
		return;
	}

	if (ctx == nullptr || ctx->mario == nullptr)
		return;

	if (!ctx->mario->GetBoundingBox().IsColliding(data.zone))
		return;

	ctx->mario->SetForceMove(data.moveTo);
	triggered = true;
}

Rect ForceVelocityTrigger::GetBoundingBox()
{
	return data.zone;
}

Vector2Int ForceVelocityTrigger::GetMoveTo()
{
	return data.moveTo;
}
