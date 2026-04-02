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
	Tile* otherTile;

	float t; // time of impact (0 < t < 1)
	float nx;
	float ny;

	CollisionEvent()
	{
		source = CollisionSource::OtherObject;
		self = nullptr;
		otherObject = nullptr;
		otherTile = nullptr;
		t = 1.0f;
		nx = 0.0f;
		ny = 0.0f;
	}
	static CollisionEvent CreateObjectCollisionEvent(GameObject* self, GameObject* other_object, float t, float nx, float ny)
	{
		CollisionEvent e;
		e.source = CollisionSource::OtherObject;
		e.self = self;
		e.otherObject = other_object;
		e.t = t;
		e.nx = nx;
		e.ny = ny;
		return e;
	}

	static CollisionEvent CreateTileCollisionEvent(GameObject* self, Tile* other_tile, float t, float nx, float ny)
	{
		CollisionEvent e;
		e.source = CollisionSource::OtherTile;
		e.self = self;
		e.otherTile = other_tile;
		e.t = t;
		e.nx = nx;
		e.ny = ny;
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
	bool collided;
	float t;
	float nx;
	float ny;

	SweptAABBResult()
	{
		collided = false;
		t = 1.0f;
		nx = 0.0f;
		ny = 0.0f;
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
	SweptAABBResult SweptAABB();
	// Helper for 2 moving object
	SweptAABBResult SweptAABB(GameObject* src, GameObject* other);

	void ProcessCollision(GameObject* go,
		const vector<GameObject*>& coObjects,
		const Tilemap* tilemap,
		DWORD dt
	);
};

