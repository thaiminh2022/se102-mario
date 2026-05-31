#pragma once
#include "GameObject.h"
class InWaterTrigger :
    public GameObject
{
	Rect zone;
	bool inWater;
	bool triggered;

public:
	InWaterTrigger(Rect zone, bool inWater);
	Rect GetBoundingBox() override;
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	bool IsCollidable() override { return false; }
};

