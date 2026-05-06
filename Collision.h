#pragma once

#include "Rect.h"
#include "Tile.h"
#include "Tilemap.h"
#include <vector>

#include "CollisionEvent.h"
#include "GameObject.h"



class Collision
{
	static Collision* _instance;

	void GetTilemapEvents(vector<CollisionEvent>& events, const Tilemap*& tilemap, GameObject
	                      *& go, float dt);
	void Filter(vector<CollisionEvent>& events, CollisionEvent*& colMinX, CollisionEvent*& colMinY, bool filterX, bool filterY);

public:
	static Collision* GetInstance()
	{
		if (_instance == nullptr)
			_instance = new Collision;

		return _instance;
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

	void GetObjectEvents(vector<CollisionEvent>& events, GameObject* go, const vector<GameObject*>& coObjects, float dt);
	void ProcessCollision(GameObject* go,
	                      const vector<GameObject*>& coObjects,
	                      const Tilemap* tilemap,
	                      float dt
	);
};

