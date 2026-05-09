#pragma once
#include "GameObject.h"
#include "Timer.h"

constexpr float BRICK_EXPLODE_SIDE_SPEED = 50.0f;
constexpr float BRICK_EXPLODE_JUMP = -150.0f;
constexpr float BRICK_GRAVITY = 900.0f;


class BrickExplode : public GameObject
{
	Timer lifeTimer;
	Vector2 topLeftDebris;
	Vector2 topRightDebris;
	Vector2 bottomLeftDebris;
	Vector2 bottomRightDebris;

	Vector2 velTopLeft;
	Vector2 velTopRight;
	Vector2 velBottomLeft;
	Vector2 velBottomRight;

	Timer spinTimer;
	bool flipState; // Toggles back and forth to simulate spinning

public:

	explicit BrickExplode(Vector2 startPosition, BiomeType biome);
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;

	bool IsCollidable() override { return false; }
	bool IsBlocking() override { return false; }
	int GetRenderIndex() override { return 1; }
	bool IsActive() override { return true; }
	Rect GetBoundingBox() override
	{
		return Rect::FromXYWH(position.x, position.y, 16, 16);
	}
};

