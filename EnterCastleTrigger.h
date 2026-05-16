#pragma once
#include "GameObject.h"

class EnterCastleTrigger : public GameObject
{
	EnterCastleTriggerData data;
	bool isTriggered;
public:
	EnterCastleTrigger(const EnterCastleTriggerData& data);
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	int GetRenderIndex() override;
	bool IsCollidable() override;
	Rect GetBoundingBox() override;
};

