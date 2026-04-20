#include "Camera.h"

#include <algorithm>

#include "Game.h"
#include "GameObject.h"


Camera::Camera()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->prevX = 0.0f;
	this->worldWidth = 0;
	this->worldHeight = 0;
	this->target = nullptr;
}

void Camera::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Camera::SetWorldSize(int width, int height)
{
	this->worldWidth = width;
	this->worldHeight = height;
}

void Camera::SetTarget(GameObject* t)
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->prevX = 0.0f;
	target = t;
}

void Camera::GetPosition(float& x, float& y) const
{
	x = this->x;
	y = this->y;
}

void Camera::WorldToScreen(float worldX, float worldY, float& ox, float& oy) const
{
	//worldX/Y is the pos of an object in the world, we want to convert it to screen pos by subtracting camera pos
	ox = worldX - x;
	oy = worldY - y;
}

void Camera::WorldToScreen(int worldX, int worldY, float& ox, float& oy) const
{
	return WorldToScreen(
		static_cast<float>(worldX),
		static_cast<float>(worldY),
		ox,
		oy);
}

void Camera::Update()
{
	auto g = Game::GetInstance();
	if (target == nullptr) return;

	auto pos = target->position;

	// center camera on target
	prevX = x;
	
	float futureX = pos.x - 0.5f * g->GetBackBufferWidth();
	if (futureX < prevX)
		return;
	x = futureX;
	
	// for now, y is fixed
	//y = targetY - 0.5f * g->GetBackBufferHeight();

	// clamp left/top
	x = max(x, 0.0f);
	y = max(y, 0.0f);

	// clamp right/bottom
	int maxX = worldWidth - g->GetBackBufferWidth();
	int maxY = worldHeight - g->GetBackBufferHeight();

	maxX = max(maxX, 0);
	maxY = max(maxY, 0);

	x = min(x, maxX);
	y = min(y, maxY);
}


bool Camera::IsInView(float left, float top, float right, float bottom) const
{
	auto g = Game::GetInstance();
	float camLeft = x;
	float camTop = y;
	float cam_ = x + g->GetBackBufferWidth();
	float camBottom = y + g->GetBackBufferHeight();

	return !(right <= camLeft ||
		left >= cam_ ||
		bottom <= camTop ||
		top >= camBottom);
}

bool Camera::IsInView(int left, int top, int right, int bottom) const
{
	return IsInView(
		static_cast<float>(left),
		static_cast<float>(top),
		static_cast<float>(right),
		static_cast<float>(bottom)
	);
}

bool Camera::IsInView(Rect r) const
{
	return IsInView(r.left, r.top, r.right, r.bottom);
}

float Camera::GetX() const { return x; }
float Camera::GetY() const { return y; }
