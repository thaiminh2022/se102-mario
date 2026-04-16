#pragma once
#include "GameObject.h"

class FireballTrap : public GameObject
{
public:
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	int GetRenderIndex() override { return 1; }
	bool IsBlocking() override { return false; }
	void OnCollisionWith(CollisionEvent* event) override;
	Rect GetBoundingBox() override
	{
		return Rect::FromXYWH(position.x, position.y, 96, 96);
	}
	~FireballTrap() override;
};

