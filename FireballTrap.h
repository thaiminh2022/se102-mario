#pragma once
#include "GameObject.h"

constexpr int MAX_FIREBALL_TRAP_COUNT = 6;

class FireballTrap : public GameObject
{
	vector<Vector2> fireballPositions;
	float currentAngle = 0;
	Vector2 center;

public:
	FireballTrap(Vector2 startPosition);
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	int GetRenderIndex() override { return 1; }
	bool IsBlocking() override { return false; }
	Rect GetBoundingBox() override
	{
		// check zone is a box
		// when player entered that box, can check whether they hit the mini fireballs
		int height;
		int width = height = MAX_FIREBALL_TRAP_COUNT * 8 * 2;

		return Rect::FromXYWH(position.x, position.y, width, height);
	}

	bool IsHitSmallBalls(Rect checkRect) const;

};


