#pragma once
#include <vector>

#include "Vector2.h"
#include "Rect.h"
#include "Scene.h"
#include <cinttypes>

#include "CollisionMatrix.h"

struct CollisionEvent;
using std::vector;


enum class CollectableItemState : std::uint8_t
{
	Emerging,
	Collectable,
	Collected,
};

class GameObject
{
protected:
	bool isDeleted;
	bool isCollidable;
	bool isBlocking;
	bool isFacingRight;	
	bool isActive;
	int renderIndex;

public:
	Vector2 position;
	Vector2 velocity;
	explicit GameObject(const float x = 0, const float y = 0) {
		position = Vector2(x, y); 
		isDeleted = false; 
		isCollidable = true; 
		isBlocking = true; 
		isFacingRight = true;
		isActive = true;
		renderIndex = 0;
	}

	explicit GameObject(const Vector2& position) {
		this->position = position;
		isDeleted = false;
		isCollidable = true;
		isBlocking = true;
		isFacingRight = true;
		isActive = true;
		renderIndex = 0;
	}


	virtual void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx = nullptr) {}
	virtual void Render() {}
	virtual int GetRenderIndex() { return renderIndex; }


	virtual bool IsCollidable() { return isCollidable; }
	virtual bool IsBlocking() { return isBlocking; }
	virtual bool IsActive() { return isActive; }
	virtual void SetActive(bool newActive) { isActive = newActive; }

	virtual void OnNoCollision(float dt) {} // Call every collision check but returns no collision
	virtual void OnCollisionWith(CollisionEvent* event) {} // Call every collision check with collision data
	virtual Rect GetBoundingBox() = 0;
	virtual CollisionMatrixLayer GetCollisionLayer() { return CollisionMatrixLayer::Default; }

	static bool IsDeleted(const GameObject* o) { return o->isDeleted; }
	virtual ~GameObject() = default;

	static bool SortRenderIndex(GameObject* a, GameObject *b)
	{
		return a->GetRenderIndex() < b->GetRenderIndex();
	}
};

