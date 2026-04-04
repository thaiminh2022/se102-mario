#include "Collision.h"
#include <algorithm>
#include "GameObject.h"
#include <queue>

using std::priority_queue;

constexpr float PUSH_BACK_FACTOR = .4f;

SweptAABBResult Collision::SweptAABB(Rect mb, float dvx, float dvy, Rect sb)
{
	float dxEntry, dyEntry;
	float dxExit, dyExit;

	// moving in x
	if (dvx > 0)
	{
		dxEntry = sb.left - mb.right;
		dxExit = sb.right - mb.left;
	}
	else
	{
		dxEntry = sb.right - mb.left;
		dxExit = sb.left - mb.right;
	}

	// moving in y

	if (dvy > 0)	
	{
		dyEntry = sb.top - mb.bottom;
		dyExit = sb.bottom - mb.top;
	}else
	{
		dyEntry = sb.bottom - mb.top;
		dyExit = sb.top - mb.bottom;
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

	float entryTime = max(txEntry, tyEntry);
	float exitTime = min(txExit, tyExit);

	SweptAABBResult result;

	if (entryTime > exitTime || (txEntry < 0.0f && tyEntry < 0.0f) || txEntry > 1.0f || tyEntry > 1.0f)
	{
		result.t = 1;
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

	return result;
}

SweptAABBResult Collision::SweptAABB(GameObject* src, GameObject* other, const float dt)
{
	auto relVelocity = src->velocity - other->velocity;
	auto dvx = relVelocity.x * dt;
	auto dvy = relVelocity.y * dt;

	return SweptAABB(src->GetBoundingBox(), dvx, dvy, other->GetBoundingBox());
}

SweptAABBResult Collision::SweptAABB(GameObject* src, CollisionTile* tile, float dt)
{
	float dvx = src->velocity.x * dt;
	float dvy = src->velocity.y * dt;

	return SweptAABB(src->GetBoundingBox(), dvx, dvy, tile->GetBounds());
}

void Collision::ProcessCollision(GameObject* go, const vector<GameObject*>& coObjects, const Tilemap* tilemap, float dt)
{
	auto srcBounds = go->GetBoundingBox();
	auto srcVelocity = go->velocity;
	vector<CollisionTile*> colTiles;
	priority_queue<CollisionEvent, vector<CollisionEvent>, CompareCollisionEvent> events;

	// tilemap collision
	if (tilemap != nullptr)
	{
		auto futureLeft = srcBounds.left + srcVelocity.x * dt;
		auto futureRight = srcBounds.right + srcVelocity.x * dt;
		auto futureTop = srcBounds.top + srcVelocity.y * dt;
		auto futureBottom = srcBounds.bottom + srcVelocity.y * dt;

		// --- FIX: Create a bounding box that covers the ENTIRE movement path ---
		float minX = min(srcBounds.left, futureLeft);
		float maxX = max(srcBounds.right, futureRight);
		float minY = min(srcBounds.top, futureTop);
		float maxY = max(srcBounds.bottom, futureBottom);


		tilemap->GetPotentialCollidableCells(RectF(
			minX,
			minY,
			maxX,
			maxY), colTiles);

		for (const auto& c : colTiles)
		{
			auto r = SweptAABB(go, c, dt);
			auto e = CollisionEvent::CreateTileCollisionEvent(go, c, r);
			events.push(e);
		}
	}

	if (events.empty())
	{
		go->OnNoCollision(dt);
		return;
	}

	CollisionEvent earliestEvent = events.top();
	if (earliestEvent.t >= 1.0f || earliestEvent.t < 0.0f)
	{
		go->OnNoCollision(dt);
		return;
	}
	go->position += srcVelocity * dt * earliestEvent.t + Vector2(earliestEvent.normalizedDir) * PUSH_BACK_FACTOR;

	if (earliestEvent.normalizedDir.x != 0)
	{
		go->velocity.x = 0.0f; // Hit a vertical wall, stop moving horizontally
	}
	if (earliestEvent.normalizedDir.y != 0)
	{
		go->velocity.y = 0.0f; // Hit a floor/ceiling, stop moving vertically
	}

	go->OnCollisionWith(&earliestEvent);
}