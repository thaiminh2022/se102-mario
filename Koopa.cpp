#include "Koopa.h"
#include "Collision.h"
#include "GameObject.h"
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

Koopa::Koopa(int startX, int startY) : GameObject(static_cast<float>(startX), static_cast<float>(startY))
{
	// Load the animations
	auto t = Textures::GetInstance()->Get(KOOPA_TEX_ID);
	auto sp = Sprites::GetInstance();
	sp->Add(KOOPA_WALK_SPRITE_1, 0, 0, 15, 15, t);
	sp->Add(KOOPA_WALK_SPRITE_2, 16, 0, 31, 15, t);

	sp->Add(FLYING_KOOPA_FLY_SPRITE_1, 0, 16, 15, 31, t);
	sp->Add(FLYING_KOOPA_FLY_SPRITE_2, 16, 16, 31, 31, t);

	sp->Add(HIDING_KOOPA_HIDE_SPRITE, 0, 32, 15, 47, t);

	sp->Add(HIDING_KOOPA_SPIN_SPRITE_1, 16, 32, 31, 47, t);
	sp->Add(HIDING_KOOPA_SPIN_SPRITE_2, 32, 32, 47, 47, t);

	sp->Add(KOOPA_DEAD_SPRITE, 48, 0, 63, 15, t);

	auto anims = Animations::GetInstance();
	if (!anims->Contains(KOOPA_WALK_ANIM_ID))
	{
		auto walkAnim = new Animation(100);
		walkAnim->Add(KOOPA_WALK_SPRITE_1);
		walkAnim->Add(KOOPA_WALK_SPRITE_2);
		anims->Add(KOOPA_WALK_ANIM_ID, walkAnim);
	}
	if (!anims->Contains(FLYING_KOOPA_FLY_ANIM_ID))
	{
		auto flyAnim = new Animation(100);
		flyAnim->Add(FLYING_KOOPA_FLY_SPRITE_1);
		flyAnim->Add(FLYING_KOOPA_FLY_SPRITE_2);
		anims->Add(FLYING_KOOPA_FLY_ANIM_ID, flyAnim);
	}
	if (!anims->Contains(HIDING_KOOPA_HIDE_ANIM_ID))
	{
		auto hideAnim = new Animation(100);
		hideAnim->Add(HIDING_KOOPA_HIDE_SPRITE);
		anims->Add(HIDING_KOOPA_HIDE_ANIM_ID, hideAnim);
	}
	if (!anims->Contains(HIDING_KOOPA_SPIN_ANIM_ID))
	{
		auto spinAnim = new Animation(100);
		spinAnim->Add(HIDING_KOOPA_SPIN_SPRITE_1);
		spinAnim->Add(HIDING_KOOPA_SPIN_SPRITE_2);
		anims->Add(HIDING_KOOPA_SPIN_ANIM_ID, spinAnim);
	}
	if (!anims->Contains(KOOPA_DEAD_ANIM_ID))
	{
		auto deadAnim = new Animation(100);
		deadAnim->Add(KOOPA_DEAD_SPRITE);
		anims->Add(KOOPA_DEAD_ANIM_ID, deadAnim);
	}
	moveLeft = false;
	state = KoopaState::Moving;
	form = KoopaForm::Normal;
	deadTimer = Timer(1.0f);
	isGrounded = true;
	moveUp = true;
}

void Koopa::SetState(KoopaState newState)
{
	state = newState;

	if (state == KoopaState::Dead || state == KoopaState::DeadUpsideDown)
	{
		isCollidable = false;
		deadTimer.Start();
		velocity.x = 0;
	}
	

	if (state == KoopaState::NotMoving)
	{
		velocity.x = 0;
	}

	if (state == KoopaState::DeadUpsideDown)
	{
		velocity.y = -150.0f;
	}
}

void Koopa::SetForm(KoopaForm newForm)
{
	form = newForm;
	if (form == KoopaForm::HiddingInShell)
	{
		SetState(KoopaState::NotMoving);
	}

}

void Koopa::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	switch (form)
	{ 	
	case KoopaForm::Normal:
		velocity.x = moveLeft ? -50.0f : 50.0f;
		break;
	case KoopaForm::Flying:
		velocity.x = moveLeft ? -50.0f : 50.0f;
		if (isGrounded)
		{
			velocity.y = -50.0f;
		}
		else
		{
			velocity.y = moveUp ? -50.0f : 50.0f;
		}
		break;
	case KoopaForm::HiddingInShell:
		if (state == KoopaState::Moving)
		{
			velocity.x = moveLeft ? -100.0f : 100.0f;
		}
		else if (state == KoopaState::NotMoving)
		{
			velocity.x = 0;
		}
		break;

		Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);
	}
}

void Koopa::Render()
{
	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);

	Animations::GetInstance()
		->Get(state == KoopaState::Dead || state == KoopaState::DeadUpsideDown ? KOOPA_DEAD_ANIM_ID :
			(form == KoopaForm::Normal ? KOOPA_WALK_ANIM_ID :
				(form == KoopaForm::Flying ? FLYING_KOOPA_FLY_ANIM_ID : HIDING_KOOPA_HIDE_ANIM_ID)))
		->Render(round(renderX), round(renderY),false,state == KoopaState::DeadUpsideDown || state == KoopaState::Dead);
}

void Koopa::OnNoCollision(float dt)
{
	position += velocity * dt;
}

void Koopa::OnCollisionWith(CollisionEvent* event)
{
	if (state == KoopaState::Dead || state == KoopaState::DeadUpsideDown)
		return;
	if (event->IsTileCollision())
	{
		if (event->otherTile->type == CollisionTileType::Death)
		{
			SetState(KoopaState::Dead);
			return;
		}
		if (event->normalizedDir.y < 0)
		{
			isGrounded = true;
			moveUp = false;
		}
		else if (event->normalizedDir.y > 0)
		{
			moveUp = true;
		}
		else if (event->normalizedDir.x != 0)
		{
			moveLeft = !moveLeft;
		}
		
	}
	else if (event->IsObjectCollision())
	{
		if (event->otherObject->IsBlocking())
		{
			if (event->normalizedDir.x != 0)
			{
				moveLeft = !moveLeft;
				if (form == KoopaForm::HiddingInShell)
				{
					SetState(KoopaState::Moving);
				}
			}
			else if (event->normalizedDir.y < 0)
			{
				isGrounded = true;
				moveUp = false;
			}
			else if (event->normalizedDir.y > 0)
			{
				moveUp = true;
			}
			
		}
		
	
	}
}