#include "Koopa.h"
#include "Collision.h"
#include "GameObject.h"
#include "Goomba.h"
#include "Scene.h"
#include "Mario.h"
#include <vector>

#include "Animation.h"
#include "Animations.h"

#include "AssetIDs.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"
#include <cmath>

#include "Debug.h"
#include "StatManager.h"

Koopa::Koopa(int startX, int startY) : GameObject(static_cast<float>(startX), static_cast<float>(startY))
{
	// Load the animations
	auto t = Textures::GetInstance()->Get(KOOPA_TEX_ID);
	auto sp = Sprites::GetInstance();
	sp->Add(KOOPA_WALK_SPRITE_1, 0, 0, 15, 23, t);
	sp->Add(KOOPA_WALK_SPRITE_2, 18, 0, 33, 23, t);

	sp->Add(WINGED_KOOPA_FLY_SPRITE_1, 36, 0, 51, 23, t);
	sp->Add(WINGED_KOOPA_FLY_SPRITE_2, 54, 0, 69, 23, t);

	sp->Add(HIDING_KOOPA_HIDE_SPRITE, 72, 8, 87, 23, t);

	sp->Add(HIDING_KOOPA_SPIN_SPRITE_1, 72, 8, 87, 23, t);
	sp->Add(HIDING_KOOPA_SPIN_SPRITE_2, 90, 8, 105, 23, t);

	sp->Add(KOOPA_DEAD_SPRITE, 72, 8, 87, 23, t);

	auto anims = Animations::GetInstance();
	if (!anims->Contains(KOOPA_WALK_ANIM_ID))
	{
		auto walkAnim = new Animation(100);
		walkAnim->Add(KOOPA_WALK_SPRITE_1);
		walkAnim->Add(KOOPA_WALK_SPRITE_2);
		anims->Add(KOOPA_WALK_ANIM_ID, walkAnim);
	}
	if (!anims->Contains(WINGED_KOOPA_FLY_ANIM_ID))
	{
		auto flyAnim = new Animation(100);
		flyAnim->Add(WINGED_KOOPA_FLY_SPRITE_1);
		flyAnim->Add(WINGED_KOOPA_FLY_SPRITE_2);
		anims->Add(WINGED_KOOPA_FLY_ANIM_ID, flyAnim);
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
	enemyKilledByShellCount = 0;
}

Koopa::Koopa(int startX, int startY, KoopaForm form) : Koopa(startX, startY)
{
	this->form = form;
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
		position.y -= 8;
		SetState(KoopaState::NotMoving);
	}

}

void Koopa::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	switch (form)
	{
	case KoopaForm::Normal:
	case KoopaForm::Winged:
		velocity.x = moveLeft ? -50.0f : 50.0f;
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
	}
	velocity.y += 500.0f * dt;
	Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);
}

void Koopa::Render()
{
	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);

	Animations::GetInstance()
		->Get(state == KoopaState::Dead || state == KoopaState::DeadUpsideDown ? KOOPA_DEAD_ANIM_ID :
			(form == KoopaForm::Normal ? KOOPA_WALK_ANIM_ID :
				(form == KoopaForm::Winged ? WINGED_KOOPA_FLY_ANIM_ID :
					(state == KoopaState::Moving ? HIDING_KOOPA_SPIN_ANIM_ID : HIDING_KOOPA_HIDE_ANIM_ID))))
		->Render(round(renderX), round(renderY), !moveLeft, state == KoopaState::DeadUpsideDown || state == KoopaState::Dead);
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
		if (event->normalizedDir.x != 0 && event->otherTile->IsBlocking())
		{
			moveLeft = !moveLeft;
		}
		if (this->form == KoopaForm::Winged)
		{
			if (event->normalizedDir.y < 0 && velocity.y >= 0.0f && event->otherTile->IsBlocking())
			{
				velocity.y = -KOOPA_JUMP_SPEED;
			}
			else if (event->normalizedDir.y > 0 && event->otherTile->IsBlocking())
			{
				position.y = event->otherTile->worldY + event->otherTile->tileHeight;
			}
		}

	}
	else if (event->IsObjectCollision())
	{
		if (this->form == KoopaForm::HiddingInShell && this->state == KoopaState::Moving)
		{
			auto sm = StatManager::GetInstance();
			// Goomba
			auto goomba = dynamic_cast<Goomba*>(event->otherObject);
			if (goomba != nullptr)
			{
				goomba->SetState(GoombaState::Dead);
				enemyKilledByShellCount++;
				sm->AddShellKillScore(enemyKilledByShellCount);
			}

			// Other Koopa
			auto koopa = dynamic_cast<Koopa*>(event->otherObject);
			if (koopa != nullptr)
			{
				if (koopa->GetState() != KoopaState::Dead && koopa->GetState() != KoopaState::DeadUpsideDown)
					if (koopa->GetForm() == KoopaForm::HiddingInShell && koopa->GetState() == KoopaState::Moving)
					{
						moveLeft = !moveLeft;// if both are moving shell, they will just bounce back without killing each other
					}
					else {
						// if the other koopa is not in moving shell state, rip bro
						koopa->SetState(KoopaState::Dead);
						enemyKilledByShellCount++;
						sm->AddShellKillScore(enemyKilledByShellCount);
					}
			}
		}

		else if (event->normalizedDir.x != 0 && event->otherObject != dynamic_cast<Mario*>(event->otherObject))
		{
			moveLeft = !moveLeft;
		}

	}
}

void Koopa::SetMoveDir(bool moveLeft)
{
	this->moveLeft = moveLeft;
}
