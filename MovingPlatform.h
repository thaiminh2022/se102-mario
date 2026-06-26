#pragma once
#include "GameObject.h"
class MovingPlatform : public GameObject
{
	MovingPlatformData data;
	int internalWidth;
	int targetPointIndex;
	bool isIncreasing;

public:
	explicit MovingPlatform(MovingPlatformData data, BiomeType biome);

	Rect GetBoundingBox() override
	{
		return Rect::FromXYWH(position.x, position.y, internalWidth, data.zone.GetHeight());
	}

	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;

	void OnNoCollision(float dt) override;

};

