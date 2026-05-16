#pragma once
#include "GameObject.h"
#include "Timer.h"

class EnterCastleTrigger : public GameObject
{
	EnterCastleTriggerData data;
	bool isTriggered;
	Timer timePerFireworkTimer;

	Vector2Int fireworkPosition;
	void SetNewFireworkPosition();

public:
	EnterCastleTrigger(const EnterCastleTriggerData& data);
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	int GetRenderIndex() override;
	bool IsCollidable() override;
	Rect GetBoundingBox() override;
};

