#include "Collision.h"
#include "GameObject.h"




void Collision::ProcessCollision(GameObject* go, const vector<GameObject*>& coObjects, const Tilemap* tilemap, DWORD dt)
{
	if (!go->IsCollidable())
		return;
	if (coObjects.empty() && tilemap == nullptr)
		return;

	auto goBox = go->GetBoundingBox();



	vector<CollisionEvent> events;
	float dtSec = dt / 1000.0f;

	// collect object collisions
	for (const auto& coObject : coObjects)
	{
		SweptAABBResult r = SweptAABB(go, coObject);
		if (r.collided)
		{
			CollisionEvent e = CollisionEvent::CreateObjectCollisionEvent(go, coObject, r.t, r.nx, r.ny);
			events.push_back(e);
		}
	}

	// tile collision
	if (tilemap != nullptr)
	{
		tilemap->GetPotentialColliableTiles();

	}
}

