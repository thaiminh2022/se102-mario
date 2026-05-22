#pragma once
#include "GameObject.h"
#include "Timer.h"

class EnterCastleTrigger : public GameObject
{
	EnterCastleTriggerData data;
	bool isTriggered;
	Timer timePerFireworkTimer;

	Vector2Int fireworkPosition;
	Vector2 flagPosition;
	bool flagFinishMoving;
	void SetNewFireworkPosition();

public:
	EnterCastleTrigger(const EnterCastleTriggerData& data, BiomeType biome);
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	int GetRenderIndex() override;
	bool IsCollidable() override;
	Rect GetBoundingBox() override;
};

