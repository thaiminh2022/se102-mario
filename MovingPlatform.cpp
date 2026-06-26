#include "MovingPlatform.h"

#include "Collision.h"
#include "Game.h"
#include "Helper.h"
#include "Sprites.h"
#include "Textures.h"


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
	if (data.movingPoints.size() <= 1)
		return;

	constexpr float minCheck = 0.1f;
	if (position.Distance(data.movingPoints[targetPointIndex]) < minCheck)
	{
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
				}
				else if (data.loopPingPong)
				{
					isIncreasing = true;
				}
			}
		}

	}else
	{
		auto dir = static_cast<Vector2>(data.movingPoints[targetPointIndex]) - position;
		dir = dir.Normalized();
		velocity = dir * data.moveSpeed;
	}
	Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);

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
		case 2:
			sp->Get(MOVING_PLATFORM_SIZE_2)->Draw(renderX, renderY, false, false);
			break;
		case 3:
			sp->Get(MOVING_PLATFORM_SIZE_3)->Draw(renderX, renderY, false, false);
			break;
		case 4:
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
