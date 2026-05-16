#pragma once
#include "GameObject.h"

enum class BlooperMoveState : std::uint8_t
{
	SwimmingUp,
	Falling,
};

class Bloopers : public GameObject
{
	Vector2 highestLimit;
	Vector2 lowestLimit;
	BlooperMoveState moveState;
	float stateTimer;
	float stateStartY;
	bool hasChosenInitialState;

	void StartSwimmingUp();
	void StartFalling();

public:
	Bloopers(Vector2Int lowestLimit, Vector2Int highestLimit, BiomeType biome);
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	bool IsBlocking() override { return false; }
	void OnNoCollision(float dt) override;
	CollisionMatrixLayer GetCollisionLayer() override { return CollisionMatrixLayer::CheepCheeps; }
	Rect GetBoundingBox() override;
};
