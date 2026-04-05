#pragma once
#include "GameObject.h"

class Camera
{
	float x, y;
	int worldWidth, worldHeight;
	GameObject* target;
public:
	Camera();
	void SetPosition(float x, float y);
	void SetWorldSize(int width, int height);
	void SetTarget(GameObject* t);

	void GetPosition(float& x, float& y) const;
	void WorldToScreen(float worldX, float worldY, float &ox, float&oy) const;

	void Update();
	bool IsInView(float left, float top, float right, float bottom) const;
};

