#pragma once
#include "GameObject.h"
#include "Timer.h"
#include <string>

class PointPopup : public GameObject
{
	std::wstring text;
	Timer lifeTimer;
	float riseSpeed;

public:
	PointPopup(Vector2 pos, int scoreValue);
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	Rect GetBoundingBox() override;
	bool IsCollidable() override { return false; }
	bool IsBlocking() override { return false; }
};
