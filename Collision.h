#pragma once
#include <queue>

#include "Tilemap.h"
#include  "Windows.h"
#include "Vector2.h"
class GameObject;

enum CollisionSource
{
	OtherObject,
	OtherTile
};

struct SweptAABBResult
{
	float t;
	bool collided;
	Vector2Int normalizeDir;


	SweptAABBResult()
	{
		collided = false;
		t = 1.0f;
	}
};



struct CollisionEvent
{
	CollisionSource source;
	GameObject* self;
	GameObject* otherObject;
	CollisionTile* otherTile;

	float t; // time of impact (0 < t < 1)
	Vector2Int normalizedDir;

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
	static CollisionEvent CreateObjectCollisionEvent(GameObject* self, GameObject* other, const SweptAABBResult& r)
	{
		CollisionEvent e;
		e.source = CollisionSource::OtherObject;
		e.self = self;
		e.otherObject = other;
		e.t = r.t;
		e.normalizedDir = r.normalizeDir;

		return e;
	}

	static CollisionEvent CreateTileCollisionEvent(GameObject* src, CollisionTile* other,  const SweptAABBResult& r)
	{
		CollisionEvent e;
		e.source = CollisionSource::OtherTile;
		e.self = src;
		e.otherTile = other;
		e.t = r.t;
		e.normalizedDir = r.normalizeDir;
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

	static bool Compare(CollisionEvent const& p1, CollisionEvent const& p2)
	{
		return p1.t < p2.t;
	}

};

class Collision
{
	void GetTilemapEvents(vector<CollisionEvent>& events, const Tilemap*& tilemap, GameObject
	                      *& go, float dt);

	void Filter(vector<CollisionEvent>& events, CollisionEvent*& colMinX, CollisionEvent*& colMinY);

public:
	static Collision* GetInstance()
	{
		static Collision _instance;
		return &_instance;
	}

	// 1 dynamic object and 1 static object (tile or non-moving object)
	SweptAABBResult SweptAABB(
		Rect mb, // moving bounds
		float dvx,
		float dvy, 
		Rect sb // static bounds
	);
	// Helper for 2 moving object
	SweptAABBResult SweptAABB(GameObject* src, GameObject* other, float dt);
	SweptAABBResult SweptAABB(GameObject* src, CollisionTile* tile, float dt);

	void ProcessCollision(GameObject* go,
		const vector<GameObject*>& coObjects,
		const Tilemap* tilemap,
		float dt
	);
};

