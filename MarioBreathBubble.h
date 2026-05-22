#pragma once
#include "GameObject.h"
class MarioBreathBubble : public GameObject
{
public:
	MarioBreathBubble(Vector2 pos);

	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	bool IsBlocking() override;
	void OnNoCollision(float dt) override;
	void OnCollisionWith(CollisionEvent* event) override;
	Rect GetBoundingBox() override;
	CollisionMatrixLayer GetCollisionLayer() override;
};

