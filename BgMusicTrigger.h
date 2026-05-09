#pragma once
#include "GameObject.h"

class BgMusicTrigger : public GameObject
{
	int musicId;
	Rect zone;
	bool triggered;
public:
	BgMusicTrigger(int musicId, Rect zone);
	Rect GetBoundingBox() override;
	bool IsCollidable() override { return false; }
	bool IsBlocking() override { return false; }
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
};

