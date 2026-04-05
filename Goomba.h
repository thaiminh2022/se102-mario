#pragma once
#include "Collision.h"
#include "GameObject.h"
#include "Rect.h"
#include "Scene.h"
#include <vector>

class Goomba : GameObject
{
public:
	Goomba(int playerStartX, int playerStartY);

	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	bool IsCollidable() override { return true; }
	bool IsBlocking() override { return false; } 
	void OnNoCollision(float dt) override;
	void OnCollisionWith(CollisionEvent* event) override;
	Rect GetBoundingBox() override
	{
		return Rect::FromXYWH(position.x, position.y, 16, 16);
	}
};

