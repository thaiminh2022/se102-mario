#include "Collision.h"
#include "GameObject.h"
#include "Goomba.h"
#include "Scene.h"
#include <vector>

#include "Animation.h"
#include "Animations.h"

#include "AssetIDs.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"
#include <cmath>

#include "Debug.h"

Goomba::Goomba(int startX, int startY) : GameObject(static_cast<float>(startX), static_cast<float>(startY))
{
	// Load the animations
	auto t = Textures::GetInstance()->Get(GOOMBA_TEX_ID);
	auto sp = Sprites::GetInstance();
	sp->Add(GOOMBA_WALK_SPRITE_1, 0, 0, 15, 15, t);
	sp->Add(GOOMBA_WALK_SPRITE_2, 16, 0, 31, 15, t);
	sp->Add(GOOMBA_DEAD_SPRITE_1, 32, 0, 47, 15, t);
	
	auto anims = Animations::GetInstance();

	if (!anims->Contains(GOOMBA_WALK_ANIM_ID))
	{
		auto walkAnim = new Animation(100);
		walkAnim->Add(GOOMBA_WALK_SPRITE_1);
		walkAnim->Add(GOOMBA_WALK_SPRITE_2);
		anims->Add(GOOMBA_WALK_ANIM_ID, walkAnim);
	}
	if (!anims->Contains(GOOMBA_DEAD_ANIM_ID))
	{
		auto deadAnim = new Animation(100);
		deadAnim->Add(GOOMBA_DEAD_SPRITE_1);
		anims->Add(GOOMBA_DEAD_ANIM_ID, deadAnim);
	}

	moveLeft = false;
	state = GoombaState::Moving;
	deadTimer = Timer(1.0f);
}

void Goomba::SetState(GoombaState newState)
{
	state = newState;

	if (state == GoombaState::Dead || state == GoombaState::DeadUpsideDown)
	{
		isCollidable = false;
		deadTimer.Start();
		velocity.x = 0;
	}

	if (state == GoombaState::DeadUpsideDown)
	{
		velocity.y = -150.0f;
	}
}

void Goomba::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (state == GoombaState::Dead || state == GoombaState::DeadUpsideDown)
	{
		deadTimer.ProcessTimer(dt);
		if (deadTimer.IsFinished())
		{
			deadTimer.SetIdle();
			isDeleted = true;
		}
	}else
	{
		
		velocity.x = moveLeft ? -50.0f : 50.0f;
	}
	velocity.y += 900 * dt;
	Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);
}

void Goomba::Render()
{
	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);



	Animations::GetInstance()
	->Get(state == GoombaState::Moving ? GOOMBA_WALK_ANIM_ID : GOOMBA_DEAD_ANIM_ID)
	->Render(round(renderX), round(renderY), false, state == GoombaState::DeadUpsideDown);
}

void Goomba::OnNoCollision(float dt)
{
	position += velocity * dt;
}

void Goomba::OnCollisionWith(CollisionEvent* event)
{
	if (state == GoombaState::Dead || state == GoombaState::DeadUpsideDown)
		return;

	if (event->IsTileCollision() 
		&& event->otherTile->IsBlocking() && event->normalizedDir.x != 0)
	{
		if (event->normalizedDir.x > 0)
		{
			moveLeft = false;

		}
		else if (event->normalizedDir.x < 0)
		{
			moveLeft = true;
		}
	}
}
