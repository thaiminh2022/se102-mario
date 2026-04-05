#include "Mario.h"

#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "Collision.h"
#include "Game.h"
#include "GameObject.h"
#include "Goomba.h"
#include "InputManager.h"
#include "Rect.h"
#include "Scene.h"
#include "Sprites.h"
#include "Textures.h"
#include <vector>

#include <cmath>

#include "NextLevelPortal.h"

Mario::Mario(int startX, int startY) : GameObject(startX, startY)

{
	auto marioTex = Textures::GetInstance()->Get(MARIO_TEX_ID);
	auto anims = Animations::GetInstance();
	auto sprites = Sprites::GetInstance();

	// sprites
	sprites->Add(MARIO_RUN_SPRITE_1, 16, 0, 31, 15, marioTex);
	sprites->Add(MARIO_RUN_SPRITE_2, 32, 0, 47, 15, marioTex);
	sprites->Add(MARIO_RUN_SPRITE_3, 48, 0, 63, 15, marioTex);
	sprites->Add(MARIO_IDLE_SPRITE_1, 0, 0, 15, 15, marioTex);

	// idle anim
	Animation* anim = new Animation(300);
	anim->Add(MARIO_IDLE_SPRITE_1);
	anims->Add(MARIO_IDLE_ANIM_ID, anim);

	// walk anim
	anim = new Animation(100);
	anim->Add(MARIO_RUN_SPRITE_1);
	anim->Add(MARIO_RUN_SPRITE_2);
	anim->Add(MARIO_RUN_SPRITE_3);
	anims->Add(MARIO_RUN_ANIM_ID, anim);

	isGrounded = false;
	isCollidable = true;
}

void Mario::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	auto input = InputManager::GetInstance();
	const float GRAVITY = 900.0f; // example
	velocity.y += GRAVITY * dt;


	if (input->IsKeyDown('A'))
	{
		velocity.x = -100.0f;
		state = MarioState::Running;
	}
	else if (input->IsKeyDown('D'))
	{
		velocity.x = 100.0f;
		state = MarioState::Running;

	}
	else
	{
		velocity.x = 0;
		state = MarioState::Idle;
	}

	if (input->IsKeyDown('W') && isGrounded)
	{
		velocity.y = -300.0f;
		isGrounded = false;
	}


	Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);

}

void Mario::Render()
{
	float renderX, renderY;
	Game::GetInstance()
		->GetCamera()
		->WorldToScreen(position.x, position.y, renderX, renderY);

	switch (state)
	{
	case MarioState::Running:
		Animations::GetInstance()->Get(MARIO_RUN_ANIM_ID)->Render(round(renderX), round(renderY));
		break;
	case MarioState::Idle:
		Animations::GetInstance()->Get(MARIO_IDLE_ANIM_ID)->Render(round(renderX), round(renderY));
		break;
	}

}

Rect Mario::GetBoundingBox()
{
	Rect r;
	r.top = position.y;
	r.left = position.x;
	r.bottom = position.y + 16;
	r.right = position.x + 16;
	return r;
}

void Mario::OnNoCollision(float dt)
{
	position += velocity * dt;
	isGrounded = false;
}

void Mario::OnCollisionWith(CollisionEvent* e)
{
	//DebugOut(L"Normal %d, %d\n", e->normalizedDir.x, e->normalizedDir.y);

	if (e->IsTileCollision()) 
	{
		// resolve tile collision
		if (e->otherTile->IsBlocking() 
			&& e->normalizedDir.y == -1
			&& e->normalizedDir.x == 0
			)
		{
			isGrounded = true;
		}
	}
	else if (e->IsObjectCollision())
	{
		// resolve object collision
		const auto goomba = dynamic_cast<Goomba*>(e->otherObject);

		if (goomba != nullptr) 
		{
			if (goomba->GetState() == GoombaState::Dead)
				return;

			if (e->normalizedDir.y == -1)
			{
				// jump on head
				velocity.y = -350.0f; // reward with free jump
				goomba->SetState(GoombaState::Dead);
			}else
			{
				// got kill by goomba, bad
				velocity.y = -100.0f;
				isCollidable = false;
			}
		}
	
		const auto portal = dynamic_cast<NextLevelPortal*>(e->otherObject);
		if (portal != nullptr)
		{
			portal->RequestNextLevel();
		}

	}
	
}
