#pragma once
#include "Collision.h"
#include "GameObject.h"
#include "Rect.h"
#include "Scene.h"
#include <vector>

enum class GoombaState : std::uint8_t
{
	Moving,
	Dead,
};

class Goomba : public GameObject
{
	bool moveLeft;
	GoombaState state;
public:
	Goomba(int startX, int startY);
	void SetState(GoombaState newState);
	GoombaState GetState() const { return state; }
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	void OnNoCollision(float dt) override;
	void OnCollisionWith(CollisionEvent* event) override;
	Rect GetBoundingBox() override
	{
		return Rect::FromXYWH(position.x, position.y, 16, 16);
	}
};

