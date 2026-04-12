#pragma once
#include "GameObject.h"
#include <cinttypes>

class Flower : public GameObject
{
	CollectableItemState state;
	Vector2 preferPos;

public:
	void SetState(CollectableItemState state);
	Flower(Vector2 startPos);
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	Rect GetBoundingBox() override;
};

