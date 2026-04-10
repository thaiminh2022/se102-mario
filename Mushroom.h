#pragma once
#include "GameObject.h"
#include "Timer.h"

enum class MushroomState
{
	Emerging,
	Moving,
	Collected,
};

class Mushroom : public GameObject
{
	MushroomState state;
	Vector2 preferPosition;

public:
	explicit Mushroom(Vector2 startPos);
	void Render() override;
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	bool IsBlocking() override { return false; }
	void OnNoCollision(float dt) override;
	Rect GetBoundingBox() override;
};


