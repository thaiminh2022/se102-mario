#include "Bloopers.h"

#include "Animations.h"
#include "AssetIDs.h"
#include "Collision.h"
#include "Debug.h"
#include "Game.h"
#include "Mario.h"
#include "Sprites.h"
#include "Textures.h"

constexpr float BLOOPER_UP_SPEED = 60.0f;
constexpr float BLOOPER_DOWN_SPEED = 60.0f;
constexpr float BLOOPER_SIDE_SPEED = 40.0f;
constexpr float BLOOPER_SHORT_SWIM_TIME = 1.0f;
constexpr float BLOOPER_SHORT_FALL_TIME = 1.0f;
constexpr float BLOOPER_MIN_SWIM_DISTANCE = 48.0f;
constexpr float BLOOPER_MIN_FALL_DISTANCE = 48.0f;
constexpr float BLOOPER_HORIZONTAL_DEAD_ZONE = 8.0f;

Bloopers::Bloopers(const Vector2Int lowestLimit, const Vector2Int highestLimit) : GameObject(lowestLimit)
{
	this->lowestLimit = Vector2(lowestLimit);
	this->highestLimit = Vector2(highestLimit);
	moveState = BlooperMoveState::SwimmingUp;
	stateTimer = 0.0f;
	stateStartY = position.y;
	hasChosenInitialState = false;

	const auto t = Textures::GetInstance()->Get(BLOOPERS_TEX_ID);
	const auto anims = Animations::GetInstance();
	const auto sp = Sprites::GetInstance();

	if (!anims->Contains(WATER_BLOOPER_IDLE_ANIM))
	{
		auto anim = new Animation();
		sp->Add(WATER_BLOOPER_IDLE_SPRITE_1, 0, 0, 15, 23, t);
		anim->Add(WATER_BLOOPER_IDLE_SPRITE_1);
		anims->Add(WATER_BLOOPER_IDLE_ANIM, anim);
	}

	if (!anims->Contains(WATER_BLOOPER_SWIM_ANIM))
	{
		auto anim = new Animation();
		sp->Add(WATER_BLOOPER_SWIM_SPRITE_1, 16, 0, 31, 15, t);
		anim->Add(WATER_BLOOPER_SWIM_SPRITE_1);
		anims->Add(WATER_BLOOPER_SWIM_ANIM, anim);
	}
}

void Bloopers::StartSwimmingUp()
{
	moveState = BlooperMoveState::SwimmingUp;
	stateTimer = 0.0f;
	stateStartY = position.y;
	DebugOut(L"Staring swim up\n");
}

void Bloopers::StartFalling()
{
	moveState = BlooperMoveState::Falling;
	stateTimer = 0.0f;
	stateStartY = position.y;
	DebugOut(L"Staring falling\n");
}

void Bloopers::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	/*
	Blooper behavior

	Blooper moves in a repeating chase cycle with two states:

	1. SwimUp
	   - Blooper swims upward in a short burst.
	   - If the player is above, Blooper targets the player's Y position.
	   - Blooper must move at least MIN_SWIM_DISTANCE before switching state.
	   - Blooper switches to FallDown when:
		 - it reaches the target Y,
		 - it reaches the upper movement limit,
		 - or it collides with the player.

	2. FallDown
	   - Blooper drifts downward.
	   - If the player is below, Blooper targets the player's Y position.
	   - Blooper must move at least MIN_FALL_DISTANCE before switching state.
	   - Blooper switches to SwimUp when:
		 - it reaches the target Y,
		 - it reaches the lower movement limit,
		 - or it collides with the player.

	General rules:
	- If the player is above the Blooper, start with SwimUp.
	- If the player is below the Blooper, start with FallDown.
	- Movement limits prevent the Blooper from leaving its allowed area.

	Special limit behavior:
	- If the player is above highestLimit.y, Blooper swims up to highestLimit.y.
	- If the player is below lowestLimit.y, Blooper falls down to lowestLimit.y.
	- In both cases, Blooper targets the nearest reachable Y position instead of the player's exact Y.
	*/
	const auto marioPosition = ctx->mario->position;

	if (!hasChosenInitialState)
	{
		if (marioPosition.y > position.y)
		{
			StartFalling();
		}
		else
		{
			StartSwimmingUp();
		}
		hasChosenInitialState = true;
	}

	stateTimer += dt;

	if (moveState == BlooperMoveState::SwimmingUp)
	{
		velocity.y = -BLOOPER_UP_SPEED;
	}
	else
	{
		velocity.y = BLOOPER_DOWN_SPEED;
	}

	const auto marioOffsetX = marioPosition.x - position.x;
	if (abs(marioOffsetX) <= BLOOPER_HORIZONTAL_DEAD_ZONE)
	{
		velocity.x = 0.0f;
	}
	else
	{
		velocity.x = marioOffsetX < 0.0f ? -BLOOPER_SIDE_SPEED : BLOOPER_SIDE_SPEED;
	}

	Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);

	if (moveState == BlooperMoveState::SwimmingUp)
	{
		const bool hitUpperLimit = position.y <= highestLimit.y;
		const bool movedMinDistance = stateStartY - position.y >= BLOOPER_MIN_SWIM_DISTANCE;

		// Mario is above the Blooper if his Y is smaller.
		const bool marioIsAbove = marioPosition.y < stateStartY;

		// If Mario is above the upper limit, target highestLimit.y instead.
		const float targetY = max(marioPosition.y, highestLimit.y);

		const bool reachedTargetY = marioIsAbove && position.y <= targetY;
		const bool completedSwimBurst = stateTimer >= BLOOPER_SHORT_SWIM_TIME;

		if (hitUpperLimit)
		{
			position.y = highestLimit.y;
			StartFalling();
		}
		else if (marioIsAbove)
		{
			// Chase upward, but still respect minimum swim distance.
			if (movedMinDistance && reachedTargetY)
			{
				StartFalling();
			}
		}
		else
		{
			if (movedMinDistance && completedSwimBurst)
			{
				StartFalling();
			}
		}
	}
	else
	{
		const bool hitLowerLimit = position.y >= lowestLimit.y;
		const bool movedMinDistance = position.y - stateStartY >= BLOOPER_MIN_FALL_DISTANCE;

		// Mario is below the Blooper if his Y is larger.
		const bool marioIsBelow = marioPosition.y > stateStartY;

		// If Mario is below the lower limit, target lowestLimit.y instead.
		const float targetY = min(marioPosition.y, lowestLimit.y);

		const bool reachedTargetY = marioIsBelow && position.y >= targetY;
		const bool completedFall = stateTimer >= BLOOPER_SHORT_FALL_TIME;

		if (hitLowerLimit)
		{
			position.y = lowestLimit.y;
			StartSwimmingUp();
		}
		else if (marioIsBelow)
		{
			// Chase downward, but still respect minimum fall distance.
			if (movedMinDistance && reachedTargetY)
			{
				StartSwimmingUp();
			}
		}
		else
		{
			if (movedMinDistance && completedFall)
			{
				StartSwimmingUp();
			}
		}
	}
}

void Bloopers::Render()
{
	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);
	
	int renderId = moveState == BlooperMoveState::SwimmingUp ? WATER_BLOOPER_SWIM_ANIM : WATER_BLOOPER_IDLE_ANIM;
	Animations::GetInstance()->Get(renderId)->Render(floor(renderX), floor(renderY), false, false);
}

void Bloopers::OnNoCollision(float dt)
{
	position += velocity * dt;
}

Rect Bloopers::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 16, moveState == BlooperMoveState::SwimmingUp ? 16 : 24);
}
