#pragma once
#include "GameObject.h"

class CastleFlag : public GameObject
{
	float targetY;
	float speed;
	bool finished;

public:
	explicit CastleFlag(Vector2 targetPosition);
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	Rect GetBoundingBox() override;
	bool IsCollidable() override { return false; }
	bool IsBlocking() override { return false; }
	bool IsFinished() const { return finished; }
	int GetRenderIndex() override { return -2; }
};
