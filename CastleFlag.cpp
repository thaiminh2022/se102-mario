#include "CastleFlag.h"

#include "AssetIDs.h"
#include "Game.h"
#include "Textures.h"
#include <cmath>

constexpr float CASTLE_FLAG_RISE_DISTANCE = 24.0f;
constexpr float CASTLE_FLAG_RISE_SPEED = 36.0f;

CastleFlag::CastleFlag(Vector2 targetPosition) : GameObject(targetPosition.x, targetPosition.y + CASTLE_FLAG_RISE_DISTANCE)
{
	targetY = targetPosition.y;
	speed = CASTLE_FLAG_RISE_SPEED;
	finished = false;
	isCollidable = false;
	isBlocking = false;
}

void CastleFlag::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (finished)
		return;

	position.y -= speed * dt;
	if (position.y <= targetY)
	{
		position.y = targetY;
		finished = true;
	}
}

void CastleFlag::Render()
{
	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);
	Game::GetInstance()->Draw(round(renderX), round(renderY), Textures::GetInstance()->Get(CASTLE_FLAG_TEX_ID), nullptr);
}

Rect CastleFlag::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 16, 16);
}
