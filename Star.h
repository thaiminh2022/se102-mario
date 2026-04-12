#pragma once
#include "GameObject.h"

class Star : public GameObject
{
	CollectableItemState state;
	Vector2 preferPos;
public:
	void SetState(CollectableItemState state);
	Star(Vector2 startPos);
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	Rect GetBoundingBox() override;
};

