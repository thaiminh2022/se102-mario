#include "Camera.h"

#include <algorithm>

#include "Game.h"
#include "GameObject.h"


Camera::Camera()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->worldWidth = 0;
	this->worldHeight = 0;
	this->target = nullptr;
}

void Camera::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Camera::SetWorldSize(float worldWidth, float worldHeight)
{
	this->worldWidth = worldWidth;
	this->worldHeight = worldHeight;
}

void Camera::SetTarget(GameObject* t)
{
	target = t;
}

void Camera::GetPosition(float& x, float& y) const
{
	x = this->x;
	y = this->y;
}

void Camera::WorldToScreen(float worldX, float worldY, float& ox, float& oy) const
{
	ox = worldX - x;
	oy = worldY - y;
}

void Camera::Update()
{
	auto g = Game::GetInstance();
	if (target == nullptr) return;

	auto pos = target->position;

	// center camera on target
	x = pos.x - 0.5f * g->GetBackBufferWidth();

	// for now, why is fixed
	//y = targetY - 0.5f * g->GetBackBufferHeight();

	// clamp left/top
	x = max(x, 0.0f);
	y = max(y, 0.0f);

	// clamp right/bottom
	float maxX = worldWidth - g->GetBackBufferWidth();
	float maxY = worldHeight - g->GetBackBufferHeight();

	maxX = max(maxX, 0.0f);
	maxY = max(maxY, 0.0f);

	x = min(x, maxX);
	y = min(y, maxY);
}


bool Camera::IsInView(float left, float top, float right, float bottom) const
{
	auto g = Game::GetInstance();
	float camLeft = x;
	float camTop = y;
	float camRight = x + g->GetBackBufferWidth();
	float camBottom = y + g->GetBackBufferHeight();

	return !(right <= camLeft ||
		left >= camRight ||
		bottom <= camTop ||
		top >= camBottom);
}