#pragma once
#include "GameObject.h"
#include "Timer.h"

enum class CheepCheepsState
{
	Moving,
	Dead,
};


class CheepCheeps : public GameObject
{
	bool isRed;
	CheepCheepsState state;
	Timer deadTimer;
public:
	CheepCheeps(Vector2Int startPos,BiomeType biome,bool isRed);
	void SetState(const CheepCheepsState s) { state = s; }
	CheepCheepsState GetState() const { return state; }
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	bool IsBlocking() override { return false; }
	void OnNoCollision(float dt) override;
	CollisionMatrixLayer GetCollisionLayer() override { return CollisionMatrixLayer::CheepCheeps; }
	Rect GetBoundingBox() override;
};

