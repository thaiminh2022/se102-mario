#pragma once
#include "Tilemap.h"
#include  "Windows.h"
#include "Vector2.h"
class GameObject;

enum CollisionSource
{
	OtherObject,
	OtherTile
};

struct CollisionEvent
{

	CollisionSource source;
	GameObject* self;
	GameObject* otherObject;
	Rect* otherTile;

	float t; // time of impact (0 < t < 1)
	Vector2 normalizedCollisionDir;

	bool isInvalid;

	CollisionEvent()
	{
		source = CollisionSource::OtherObject;
		self = nullptr;
		otherObject = nullptr;
		otherTile = nullptr;
		t = 1.0f;
		isInvalid = false;
	}
	static CollisionEvent CreateObjectCollisionEvent(GameObject* self, GameObject* other_object, float t, Vector2 normalizedCollisionDir)
	{
		CollisionEvent e;
		e.source = CollisionSource::OtherObject;
		e.self = self;
		e.otherObject = other_object;
		e.t = t;
		e.normalizedCollisionDir = normalizedCollisionDir;

		return e;
	}

	static CollisionEvent CreateTileCollisionEvent(GameObject* self, Rect* other_tile, float t, Vector2 normalizedCollisionDir)
	{
		CollisionEvent e;
		e.source = CollisionSource::OtherTile;
		e.self = self;
		e.otherTile = other_tile;
		e.t = t;
		e.normalizedCollisionDir = normalizedCollisionDir;
		return e;
	}


	bool IsTileCollision() const
	{
		return source == OtherTile;
	}
	bool IsObjectCollision() const
	{
		return source == OtherObject;
	}

	static bool Compare(const CollisionEvent& a, const CollisionEvent& b)
	{
		return a.t < b.t;
	}
};



struct SweptAABBResult
{
	float t;
	bool collided;
	Vector2 normalizedCollisionDir;
	

	SweptAABBResult()
	{
		collided = false;
		t = 1.0f;
		normalizedCollisionDir = Vector2();
	}
};
class Collision
{
public:
	static Collision* GetInstance()
	{
		static Collision _instance;
		return &_instance;
	}

	// 1 dynamic object and 1 static object (tile or non-moving object)
	SweptAABBResult SweptAABB(
		Rect mb, // moving bounds
		float mDeltaVelocityX,
		float mDeltaVelocityY, 
		Rect sb // static bounds
	);
	// Helper for 2 moving object
	SweptAABBResult SweptAABB(GameObject* src, GameObject* other, float dtSec);
	SweptAABBResult SweptAABB(GameObject* src, Rect tile, float dtSec);

	void ProcessCollision(GameObject* go,
		const vector<GameObject*>& coObjects,
		const Tilemap* tilemap,
		DWORD dt
	);
};

