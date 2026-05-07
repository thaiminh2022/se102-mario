#pragma once
#include "GameObject.h"

class ClearScreenColorTrigger : public GameObject
{
	Rect zone;
	Color color;
	bool triggered;
public:
	ClearScreenColorTrigger(Rect z, Color c);
	Rect GetBoundingBox() override;
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	bool IsCollidable() override { return false; }
	bool IsBlocking() override { return false; }
};

