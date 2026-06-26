#include "MovingPlatform.h"

#include <cmath>

#include "Game.h"
#include "Helper.h"
#include "Mario.h"
#include "Sprites.h"
#include "Textures.h"


bool IsStandingOn(const Rect& rider, const Rect& platform)
{
	constexpr int topSlack = 2;
	const bool overlapsX = rider.right >= platform.left && rider.left <= platform.right;
	const bool onTop = std::abs(rider.bottom - platform.top) <= topSlack;
	return overlapsX && onTop;
}

MovingPlatform::MovingPlatform(MovingPlatformData data, BiomeType biome) : GameObject(data.zone.left, data.zone.top), data(std::move(data))
{
	auto t = Textures::GetInstance()->Get(ChooseTilesetId(biome));
	auto sp = Sprites::GetInstance();

	auto width = this->data.zone.GetWidth() / 8;

	if (width <= 2)
	{
		sp->Add(MOVING_PLATFORM_SIZE_2, 0, 128, 15, 135, t);
		internalWidth = 2 * 8;
	}
	else if (width <= 3)
	{
		sp->Add(MOVING_PLATFORM_SIZE_3, 16, 128, 39, 135, t);
		internalWidth = 3 * 8;

	}else if (width <= 4)
	{
		sp->Add(MOVING_PLATFORM_SIZE_4, 48, 128, 79, 135, t);
		internalWidth = 4 * 8;

	}else
	{
		sp->Add(MOVING_PLATFORM_SIZE_6, 80, 128, 127, 135, t);
		internalWidth = 6 * 8;
	}
	isIncreasing = true;
	targetPointIndex = 0;

}

void MovingPlatform::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (data.movingPoints.size() <= 1 || dt <= 0.0f)
	{
		frameDelta = Vector2::Zero();
		return;
	}

	const auto oldPosition = position;
	const auto oldBounds = GetBoundingBox();
	constexpr float minCheck = 0.1f;
	if (position.Distance(data.movingPoints[targetPointIndex]) < minCheck)
	{
		velocity = Vector2::Zero();
		if (isIncreasing) {

			// still have point
			if (data.movingPoints.size() > targetPointIndex + 1)
			{
				targetPointIndex++;
			}
			else
			{
				if (data.loop)
				{
					targetPointIndex = 0;
					position = data.movingPoints[targetPointIndex];
					frameDelta = position - oldPosition;
				}else if (data.loopPingPong)
				{
					isIncreasing = false;
				}
			}
		}else
		{
			// still have point
			if (targetPointIndex -1 >= 0)
			{
				targetPointIndex--;
			}
			else
			{
				if (data.loop)
				{
					targetPointIndex = data.movingPoints.size() - 1;
					position = data.movingPoints[targetPointIndex];
					frameDelta = position - oldPosition;
				}
				else if (data.loopPingPong)
				{
					isIncreasing = true;
				}
			}
		}

	}else
	{
		const auto target = static_cast<Vector2>(data.movingPoints[targetPointIndex]);
		const auto toTarget = target - position;
		const auto maxDistance = data.moveSpeed * dt;
		if (toTarget.Length() <= maxDistance)
		{
			velocity = toTarget / dt;
			position = target;
			frameDelta = position - oldPosition;
		}
		else
		{
			auto dir = toTarget.Normalized();
			velocity = dir * data.moveSpeed;
			position += velocity * dt;
		}
	}
	frameDelta = position - oldPosition;
	const auto newBounds = GetBoundingBox();

	for (auto other : coObjects)
	{
		auto mario = dynamic_cast<Mario*>(other);
		if (mario != nullptr && IsStandingOn(mario->GetBoundingBox(), oldBounds))
		{
			mario->MoveWithPlatform(frameDelta, newBounds);
		}
	}
}

void MovingPlatform::Render()
{
	const auto sp = Sprites::GetInstance();
	float renderX, renderY;

	Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);
	
	renderX = round(renderX);
	renderY = round(renderY);
	switch (internalWidth)
	{
		case 16:
			sp->Get(MOVING_PLATFORM_SIZE_2)->Draw(renderX, renderY, false, false);
			break;
		case 24:
			sp->Get(MOVING_PLATFORM_SIZE_3)->Draw(renderX, renderY, false, false);
			break;
		case 32:
			sp->Get(MOVING_PLATFORM_SIZE_4)->Draw(renderX, renderY, false, false);
			break;
		default:
			sp->Get(MOVING_PLATFORM_SIZE_6)->Draw(renderX, renderY, false, false);
			break;
	}
}

void MovingPlatform::OnNoCollision(float dt)
{
	position += velocity * dt;
}
