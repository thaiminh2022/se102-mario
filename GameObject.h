#pragma once
#include <Windows.h>
#include <vector>

#include "Collision.h"
#include "Vector2.h"

using std::vector;

class GameObject
{
protected:
	bool isDeleted;

public:
	Vector2 position;
	Vector2 velocity;
	explicit GameObject(float x = 0, float y = 0) { position = Vector2(x, y); isDeleted = false; }

	virtual void Update(DWORD dt, vector<GameObject*>* coObjects = nullptr) = 0;
	virtual void Render() = 0;
	
	virtual bool IsCollidable() { return true; }
	virtual bool IsBlocking() { return true; }
	virtual void OnNoCollision(DWORD dt) {} // Call every collision check but returns no collision
	virtual void OnCollisionWith(CollisionEvent* event) {} // Call every collision check with collision data
	virtual  RECT GetBoundingBox() = 0;

	static bool IsDeleted(const GameObject* o) { return o->isDeleted; }
	virtual ~GameObject() = default;
};

