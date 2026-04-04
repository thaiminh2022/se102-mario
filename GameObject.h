#pragma once
#include <Windows.h>
#include <vector>

#include "Collision.h"
#include "Vector2.h"
#include "Rect.h"
#include "Scene.h"

using std::vector;

class GameObject
{
protected:
	bool isDeleted;

public:
	Vector2 position;
	Vector2 velocity;
	explicit GameObject(float x = 0, float y = 0) { position = Vector2(x, y); isDeleted = false; }

	virtual void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx = nullptr) = 0;
	virtual void Render() = 0;

	virtual bool IsCollidable() { return true; }
	virtual bool IsBlocking() { return true; }
	virtual void OnNoCollision(float dt) {} // Call every collision check but returns no collision
	virtual void OnCollisionWith(CollisionEvent* event) {} // Call every collision check with collision data
	virtual Rect GetBoundingBox() = 0;

	static bool IsDeleted(const GameObject* o) { return o->isDeleted; }
	virtual ~GameObject() = default;
};

