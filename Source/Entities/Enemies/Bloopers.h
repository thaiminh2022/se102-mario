#pragma once
#include "GameObject.h"
#include "Timer.h"

enum class BlooperState : std::uint8_t
{
	SwimmingUp,
	Falling,
	Dead,
};


class Bloopers : public GameObject
{
	Vector2 highestLimit;
	Vector2 lowestLimit;
	BlooperState moveState;
	float stateTimer;
	float stateStartY;
	bool hasChosenInitialState;
	Timer deadTimer;


	void StartSwimmingUp();
	void StartFalling();
public:
	Bloopers(Vector2Int lowestLimit, Vector2Int highestLimit, BiomeType biome);
	void SetState(BlooperState s);
	BlooperState GetState() const { return moveState; }
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	bool IsBlocking() override { return false; }
	void OnNoCollision(float dt) override;
	CollisionMatrixLayer GetCollisionLayer() override { return CollisionMatrixLayer::CheepCheeps; }
	Rect GetBoundingBox() override;
};
