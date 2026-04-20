#include "Collision.h"

#include <algorithm>

#include "GameObject.h"

#include "Rect.h"
#include "Tile.h"
#include "Tilemap.h"
#include "Vector2.h"
#include <cmath>
#include <vector>
#include <Windows.h>

#include "Game.h"

constexpr float PUSH_BACK_FACTOR = 0.001f;
Collision* Collision::_instance = nullptr;


void Collision::GetTilemapEvents(vector<CollisionEvent>& events, const Tilemap*& tilemap, GameObject*& go, float dt)
{
	// tilemap collision
	if (tilemap != nullptr)
	{
		auto srcBounds = go->GetBoundingBox();
		auto srcVelocity = go->velocity;
		vector<CollisionTile*> collisionTiles;
		
		auto futureLeft = srcBounds.left + srcVelocity.x * dt;
		auto futureRight = srcBounds.right + srcVelocity.x * dt;
		auto futureTop = srcBounds.top + srcVelocity.y * dt;
		auto futureBottom = srcBounds.bottom + srcVelocity.y * dt;

		float minX = min(srcBounds.left, futureLeft);
		float maxX = max(srcBounds.right, futureRight);
		float minY = min(srcBounds.top, futureTop);
		float maxY = max(srcBounds.bottom, futureBottom);

		auto r = RectF(
			minX,
			minY,
			maxX,
			maxY);

		//Game::GetInstance()->DrawDebugRectWithCamera(r, Colors::YELLOW);
		tilemap->GetPotentialCollidableCells(r, collisionTiles);

		if (collisionTiles.empty())
			return;

		for (const auto& c : collisionTiles)
		{
			auto r = SweptAABB(go, c, dt);
			auto e = CollisionEvent::CreateTileCollisionEvent(go, c, r);
			events.push_back(e);
		}
	}
}

void Collision::Filter(
	vector<CollisionEvent>& events,
	CollisionEvent*& colMinX,
	CollisionEvent*& colMinY,
	bool filterX = true,
	bool filterY = true
)
{
	float minXTime = 1.0f;
	float minYTime = 1.0f;
	
	for (auto& v : events)
	{
		if (v.isInvalid) continue;
		if (!v.IsBlocking()) continue;
		if (v.self == nullptr || GameObject::IsDeleted(v.self)) continue;
		if (v.t < 0 || v.t > 1) continue;


		// Hit oneway tile, so ignore
		if (v.IsTileCollision() && v.otherTile->type == CollisionTileType::OneWay)
		{
			// We only care if the player is landing on the TOP.
			// If the collision normal is anything else (Side or Bottom), 
			// we ignore it entirely so the player passes through.
			if (v.normalizedDir.y != -1)
			{
				continue;
			}
		}

		if (colMinX == nullptr && v.normalizedDir.x != 0 && minXTime > v.t && filterX)
		{
			colMinX = &v;
			minXTime = v.t;
		}

		if (colMinY == nullptr && v.normalizedDir.y != 0 && minYTime > v.t && filterY)
		{
			colMinY = &v;
			minYTime = v.t;
		}

		if (colMinX  != nullptr && colMinY != nullptr)
			break;
	}
}

SweptAABBResult Collision::SweptAABB(Rect mb, float dvx, float dvy, Rect sb)
{
	float dxEntry, dyEntry;
	float dxExit, dyExit;

	// moving in x
	if (dvx > 0)
	{
		dxEntry = static_cast<float>(sb.left - mb.right);
		dxExit = static_cast<float>(sb.right - mb.left);
	}
	else
	{
		dxEntry = static_cast<float>(sb.right - mb.left);
		dxExit = static_cast<float>(sb.left - mb.right);
	}

	// moving in y

	if (dvy > 0)
	{
		dyEntry = static_cast<float>(sb.top - mb.bottom);
		dyExit = static_cast<float>(sb.bottom - mb.top);
	}else
	{
		dyEntry = static_cast<float>(sb.bottom - mb.top);
		dyExit = static_cast<float>(sb.top - mb.bottom);
	}
	float txEntry, tyEntry;
	float txExit, tyExit;
	if (dvx == 0.0f)
	{
		// If we are not overlapping on X, we can never collide!
		if (mb.right <= sb.left || mb.left >= sb.right)
		{
			txEntry = INFINITY;
			txExit = -INFINITY;
		}
		else
		{
			txEntry = -INFINITY;
			txExit = INFINITY;
		}
	}else
	{
		txEntry = dxEntry / dvx;
		txExit = dxExit / dvx;
	}

	if (dvy == 0.0f)
	{
		// If we are not overlapping on Y, we can never collide!
		if (mb.bottom <= sb.top || mb.top >= sb.bottom)
		{
			tyEntry = INFINITY;
			tyExit = -INFINITY;
		}
		else
		{
			tyEntry = -INFINITY;
			tyExit = INFINITY;
		}
	}
	else
	{
		tyEntry = dyEntry / dvy;
		tyExit = dyExit / dvy;
	}

	const float entryTime = max(txEntry, tyEntry);
	const float exitTime = min(txExit, tyExit);

	SweptAABBResult result;

	if (entryTime > exitTime || (txEntry < 0.0f && tyEntry < 0.0f) || txEntry > 1.0f || tyEntry > 1.0f)
	{
		result.t = 1;
		result.collided = false;
		return result;
	}
	result.t = entryTime;
	if (txEntry > tyEntry)
	{
		result.normalizeDir = dvx > 0.0f ? Vector2Int::Left() : Vector2Int::Right();
	}else
	{
		result.normalizeDir = dvy > 0.0f ? Vector2Int::Up() : Vector2Int::Down();
	}
	result.collided = true;
	return result;
}

SweptAABBResult Collision::SweptAABB(GameObject* src, GameObject* other, const float dt)
{
	const auto relVelocity = src->velocity - other->velocity;
	const auto dvx = relVelocity.x * dt;
	const auto dvy = relVelocity.y * dt;

	return SweptAABB(src->GetBoundingBox(), dvx, dvy, other->GetBoundingBox());
}

SweptAABBResult Collision::SweptAABB(GameObject* src, CollisionTile* tile, float dt)
{
	float dvx = src->velocity.x * dt;
	float dvy = src->velocity.y * dt;

	return SweptAABB(src->GetBoundingBox(), dvx, dvy, tile->GetBounds());
}

void Collision::GetObjectEvents(vector<CollisionEvent>& events, GameObject* go, const vector<GameObject*>& coObjects,
	const float dt)
{

	for (const auto& obj : coObjects)
	{
		if (obj == go || !obj->IsCollidable() || GameObject::IsDeleted(obj))
			continue;

		auto r = SweptAABB(go, obj, dt);
		if (r.collided)
		{
			auto e = CollisionEvent::CreateObjectCollisionEvent(go, obj, r);
			events.push_back(e);
		}
	}
}

void Collision::ProcessCollision(GameObject* go, const vector<GameObject*>& coObjects, const Tilemap* tilemap, float dt)
{
	if (!go->IsCollidable())
	{
		go->OnNoCollision(dt);
		return;
	}

	bool collidedX = false, collidedY = false;

	// Keep track of non-blocking entities we touch this frame 
	// to prevent triggering them twice (once in X, once in Y)
	// Yes, i use void*, fight me idc
	std::vector<void*> touchedNonBlocking;

	// ==========================================
	// STEP 1: MOVE AND RESOLVE X-AXIS ONLY
	// ==========================================
	
	// Temporarily shutdown y velocity to solve x
	float originalVy = go->velocity.y;
	go->velocity.y = 0;

	vector<CollisionEvent> eventsX;
	GetTilemapEvents(eventsX, tilemap, go, dt);
	GetObjectEvents(eventsX, go, coObjects, dt);

	if (!eventsX.empty())
	{
		CollisionEvent* colX = nullptr;
		CollisionEvent* dummyY = nullptr;
		Filter(eventsX, colX, dummyY, true, false);

		// solve X
		if (colX != nullptr)
		{
			collidedX = true;
			go->position.x += go->velocity.x * dt * colX->t + colX->normalizedDir.x * PUSH_BACK_FACTOR;
			go->velocity.x = 0;
			go->OnCollisionWith(colX);
		}
		else
		{
			go->position.x += go->velocity.x * dt;
		}

		// Handle non-blocking events for X
		for (auto& v : eventsX)
		{
			if (!v.isInvalid && !v.IsBlocking())
			{
				// Get a generic pointer to whatever we hit (Tile or Object)
				void* entity = v.IsTileCollision() ? static_cast<void*>(v.otherTile) : static_cast<void*>(v.otherObject);

				// check if we resolve this before
				if (std::find(touchedNonBlocking.begin(), touchedNonBlocking.end(), entity) == touchedNonBlocking.end())
				{
					go->OnCollisionWith(&v);
					touchedNonBlocking.push_back(entity);
				}
			}
		}
	}
	else
	{
		go->position.x += go->velocity.x * dt;
	}

	eventsX.clear();

	// ==========================================
	// STEP 2: MOVE AND RESOLVE Y-AXIS ONLY
	// ==========================================

	// shutdown x to solve for y, also restore y
	go->velocity.y = originalVy;
	float originalVx = go->velocity.x;
	go->velocity.x = 0;

	vector<CollisionEvent> eventsY;
	GetTilemapEvents(eventsY, tilemap, go, dt);
	GetObjectEvents(eventsY, go, coObjects, dt);

	if (!eventsY.empty())
	{
		CollisionEvent* dummyX = nullptr;
		CollisionEvent* colY = nullptr;
		Filter(eventsY, dummyX, colY, false, true);
			
		if (colY != nullptr)
		{
			collidedY = true;
			go->position.y += go->velocity.y * dt * colY->t + colY->normalizedDir.y * PUSH_BACK_FACTOR;
			go->velocity.y = 0;
			go->OnCollisionWith(colY);
		}
		else
		{
			go->position.y += go->velocity.y * dt;
		}

		// Handle non-blocking events for Y
		for (auto& v : eventsY)
		{
			if (!v.isInvalid && !v.IsBlocking())
			{
				void* entity = v.IsTileCollision() ? (void*)v.otherTile : (void*)v.otherObject;

				// Only trigger if we didn't already touch it during the X sweep
				if (std::find(touchedNonBlocking.begin(), touchedNonBlocking.end(), entity) == touchedNonBlocking.end())
				{
					go->OnCollisionWith(&v);
					touchedNonBlocking.push_back(entity);
				}
			}
		}
	}
	else
	{
		go->position.y += go->velocity.y * dt;
	}
	// Restore the original X velocity (it will be 0 if we hit a wall in Step 1, which is correct)
	go->velocity.x = originalVx;
	eventsY.clear();

	if (!collidedX && !collidedY)
	{
		go->OnNoCollision(dt);
	}

}
