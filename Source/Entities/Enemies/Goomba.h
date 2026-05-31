#pragma once
#include "GameObject.h"
#include "Rect.h"
#include "Scene.h"
#include <vector>
#include "Timer.h"

enum class GoombaState : std::uint8_t
{
	Moving,
	Dead,
	DeadUpsideDown
};

class Goomba : public GameObject
{
	bool moveLeft;
	GoombaState state;
	Timer deadTimer;
public:
	Goomba(Vector2Int startPos, BiomeType biome);
	void SetState(GoombaState newState);
	GoombaState GetState() const { return state; }
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	void OnNoCollision(float dt) override;
	void OnCollisionWith(CollisionEvent* event) override;
	Rect GetBoundingBox() override
	{
		return Rect::FromXYWH(static_cast<int>(position.x+3), static_cast<int>(position.y) + 5, 11, 11);
	}
	bool IsBlocking() override
	{
		return false;
	}
};

