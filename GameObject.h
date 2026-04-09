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
	bool isCollidable;
	bool isBlocking;
	bool isFacingRight;	
public:
	Vector2 position;
	Vector2 velocity;
	explicit GameObject(float x = 0, float y = 0) {
		position = Vector2(x, y); isDeleted = false; isCollidable = true; isBlocking = true; isFacingRight = true;
	}

	virtual void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx = nullptr) {};
	virtual void Render() {}

	virtual bool IsCollidable() { return isCollidable; }
	virtual bool IsBlocking() { return isBlocking; }
	virtual void OnNoCollision(float dt) {} // Call every collision check but returns no collision
	virtual void OnCollisionWith(CollisionEvent* event) {} // Call every collision check with collision data
	virtual Rect GetBoundingBox() = 0;

	static bool IsDeleted(const GameObject* o) { return o->isDeleted; }
	virtual ~GameObject() = default;
};

