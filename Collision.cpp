#include "Collision.h"
#include <algorithm>
#include <cmath>
#include "Debug.h"
#include "GameObject.h"

constexpr float PUSH_BACK_FACTOR = 0.1f;

SweptAABBResult Collision::SweptAABB(Rect mb, float mDeltaVelocityX, float mDeltaVelocityY, Rect sb)
{
	return {};
}

SweptAABBResult Collision::SweptAABB(GameObject* src, GameObject* other, const float dtSec)
{
	return {};

}

SweptAABBResult Collision::SweptAABB(GameObject* src, Rect tile, float dtSec)
{
	return {};

}

void Collision::ProcessCollision(GameObject* go, const vector<GameObject*>& coObjects, const Tilemap* tilemap, DWORD dt)
{
	go->OnNoCollision(dt);
}