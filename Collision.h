#pragma once
#include "Tilemap.h"
#include  "Windows.h"
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
		self = nullptr;
		source = CollisionSource::OtherObject;
		otherObject = nullptr;
		otherTile = nullptr;
		t = 1.0f;
		nx = 0.0f;
		ny = 0.0f;
	}

	bool IsTileCollision() const
	{
		return source == OtherTile;
	}
	bool IsObjectCollision() const
	{
		return source == OtherObject;
	}
};

struct SweptAABBResult
{
	
};

struct AABB
{
	
};

class Collision
{
	static Collision* _instance;

public:
	static Collision* GetInstance()
	{
		if (_instance  == nullptr)
		{
			_instance = new Collision;
		}
		return _instance;
	}

	void ProcessCollision(GameObject* go, 
		const vector<GameObject*>*& coObjects, 
		const Tilemap*& tilemap, 
		DWORD dt
	);
};

