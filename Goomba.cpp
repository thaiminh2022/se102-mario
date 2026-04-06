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
	
	auto walkAnim = new Animation(100);
	walkAnim->Add(GOOMBA_WALK_SPRITE_1);
	walkAnim->Add(GOOMBA_WALK_SPRITE_2);
	anims->Add(GOOMBA_WALK_ANIM_ID, walkAnim);

	auto deadAnim = new Animation(100);
	deadAnim->Add(GOOMBA_DEAD_SPRITE_1);
	anims->Add(GOOMBA_DEAD_ANIM_ID, deadAnim);

	moveLeft = true;
	state = GoombaState::Moving;
}

void Goomba::SetState(GoombaState newState)
{
	state = newState;

	if (state == GoombaState::Dead)
	{
		isCollidable = false;
		//isDeleted = true;  need a timer before delete
	}
}

void Goomba::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (state == GoombaState::Dead)
		return;

	velocity.y += 900 * dt;
	// default move to left
	velocity.x = moveLeft ? 50 : -50.0f;

	Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);
}

void Goomba::Render()
{
	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);

	Animations::GetInstance()
	->Get(state == GoombaState::Moving ? GOOMBA_WALK_ANIM_ID : GOOMBA_DEAD_ANIM_ID)
	->Render(round(renderX), round(renderY));
}

void Goomba::OnNoCollision(float dt)
{
	if (state == GoombaState::Dead)
		return;

	position += velocity * dt;
}

void Goomba::OnCollisionWith(CollisionEvent* event)
{
	if (state == GoombaState::Dead)
		return;

	if (event->IsTileCollision() 
		&& event->otherTile->IsBlocking() && event->normalizedDir.x != 0)
	{
		if (event->normalizedDir.x > 0)
		{
			moveLeft = true;
			DebugOut(L"Move left true\n");

		}
		else if (event->normalizedDir.x < 0)
		{
			moveLeft = false;
			DebugOut(L"Move left false\n");
		}
	}
}
