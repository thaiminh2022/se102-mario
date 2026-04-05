#include "Collision.h"
#include <algorithm>
#include "GameObject.h"

#include "Game.h"


constexpr float PUSH_BACK_FACTOR = 0.1f;

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

		float minX = min(srcBounds.left, futureLeft -16);
		float maxX = max(srcBounds.right, futureRight + 16);
		float minY = min(srcBounds.top, futureTop - 16);
		float maxY = max(srcBounds.bottom, futureBottom + 16);

		auto r = RectF(
			minX,
			minY,
			maxX,
			maxY);

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

void Collision::Filter(vector<CollisionEvent>& events, CollisionEvent*& colMinX, CollisionEvent*& colMinY)
{
	float minXTime = 1.0f;
	float minYTime = 1.0f;
	
	for (auto& v : events)
	{
		if (v.isInvalid) continue;
		if (v.self == nullptr || GameObject::IsDeleted(v.self)) continue;
		if (v.t < 0 || v.t >= 1) continue;

		if (colMinX == nullptr && v.normalizedDir.x != 0 && minXTime > v.t)
		{
			colMinX = &v;
			minXTime = v.t;
		}

		if (colMinY == nullptr && v.normalizedDir.y != 0 && minYTime > v.t)
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
	vector<CollisionEvent> events;
	GetTilemapEvents(events, tilemap, go, dt);

	std::sort(events.begin(), events.end(), CollisionEvent::Compare);

	if (events.empty())
	{
		go->OnNoCollision(dt);
		return;
	}

	CollisionEvent* colX = nullptr, *colY = nullptr;
	Filter(events, colX, colY);

	auto position = go->position;
	auto srcVelocity = go->velocity;


	if (colX != nullptr && colY != nullptr)
	{
		// have collision on x, y
		if (colX->t < colY->t)
		{
			// x happen first
			position.x += srcVelocity.x * dt * colX->t + colX->normalizedDir.x * PUSH_BACK_FACTOR;
			go->position = position;
			go->OnCollisionWith(colX);
			go->velocity.x = 0;

			// ok after move x, is there still collision on y
			colY->isInvalid = true;

			CollisionEvent* colYOther = nullptr;
			if (colY->IsTileCollision())
			{
				const auto r = SweptAABB(go, colY->otherTile, dt);
				events.push_back(CollisionEvent::CreateTileCollisionEvent(go, colY->otherTile, r));
			}else
			{
				const auto r = SweptAABB(go, colY->otherObject, dt);
				events.push_back(CollisionEvent::CreateObjectCollisionEvent(go, colY->otherObject, r));
			}

			Filter(events, colX, colYOther);
			if (colYOther != nullptr)
			{
				position.y += srcVelocity.y * dt * colYOther->t + colYOther->normalizedDir.y * PUSH_BACK_FACTOR;
				go->OnCollisionWith(colYOther);
			}
			else
			{
				position.y += srcVelocity.y * dt;
			}
		}else
		{
			// col y happens first
			position.y += srcVelocity.y * dt * colY->t + colY->normalizedDir.y * PUSH_BACK_FACTOR;
			go->position = position;
			go->OnCollisionWith(colY);
			go->velocity.y = 0;

			// ok after move y, is there still collision on x?
			colX->isInvalid = true;

			CollisionEvent* colXOther = nullptr;
			if (colX->IsTileCollision())
			{
				const auto r = SweptAABB(go, colX->otherTile, dt);
				events.push_back(CollisionEvent::CreateTileCollisionEvent(go, colX->otherTile, r));
			}
			else
			{
				const auto r = SweptAABB(go, colX->otherObject, dt);
				events.push_back(CollisionEvent::CreateObjectCollisionEvent(go, colX->otherObject, r));
			}

			Filter(events, colXOther, colY);
			if (colXOther != nullptr)
			{
				position.x += srcVelocity.x * dt * colXOther->t + colXOther->normalizedDir.x * PUSH_BACK_FACTOR;
				go->OnCollisionWith(colXOther);
			}
			else
			{
				position.x += srcVelocity.x * dt;
			}
		}

	}
	else if (colX != nullptr)
	{
		// have collision on x only
		position.x += srcVelocity.x * dt * colX->t + colX->normalizedDir.x * PUSH_BACK_FACTOR;
		position.y += srcVelocity.y * dt;
		go->velocity.x = 0;
		go->OnCollisionWith(colX);

	}else if (colY != nullptr)
	{
		// have collision on y only
		position.y += srcVelocity.y * dt * colY->t + colY->normalizedDir.y * PUSH_BACK_FACTOR;
		position.x += srcVelocity.x * dt;
		go->velocity.y = 0;
		go->OnCollisionWith(colY);
	}else
	{
		position += srcVelocity * dt;
	}
	go->position = position;

	events.clear();

}