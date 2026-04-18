#pragma once
#include "GameObject.h"

class FlagPole : public GameObject
{
	Rect zone;
	Vector2Int playerMoveTo;
	Vector2 flagPosition;

public:
	FlagPole(Rect r, Vector2Int moveTo);
	
	Rect GetBoundingBox() override;
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	bool IsBlocking() override { return false; }
	void OnCollisionWith(CollisionEvent* event) override;
};

