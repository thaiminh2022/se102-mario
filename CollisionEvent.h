#pragma once
#include "GameObject.h"
#include  "Tile.h"

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

enum class CollisionSource
{
	OtherObject,
	OtherTile
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

	static CollisionEvent CreateTileCollisionEvent(GameObject* src, CollisionTile* other, const SweptAABBResult& r)
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
		return source == CollisionSource::OtherTile;
	}
	bool IsObjectCollision() const
	{
		return source == CollisionSource::OtherObject;
	}

	bool IsBlocking() const
	{
		return (IsTileCollision() && otherTile->IsBlocking()) ||
			(IsObjectCollision() && otherObject->IsBlocking());
	}

	static bool Compare(CollisionEvent const& p1, CollisionEvent const& p2)
	{
		return p1.t < p2.t;
	}

};
