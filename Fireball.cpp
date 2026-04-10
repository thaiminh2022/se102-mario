#include "Fireball.h"
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
#include <algorithm>
#include <vector>

#include <cmath>
#include <string>

#include "Debug.h"
#include "AudioManager.h"
#include "FontManager.h"
#include "NextLevelPortal.h"



Fireball::Fireball(float x, float y, bool isFacingRight)
{
	auto sprites = Sprites::GetInstance();
	auto anims = Animations::GetInstance();
	auto tex = Textures::GetInstance()->Get(FIREBALL_TEX_ID);

	// BOUNCING
	sprites->Add(FIREBALL_BOUNCE_SPRITE_1, 0, 8, 7, 15, tex);
	sprites->Add(FIREBALL_BOUNCE_SPRITE_2, 10, 8, 17, 15, tex);
	sprites->Add(FIREBALL_BOUNCE_SPRITE_3, 20, 8, 27, 15, tex);
	sprites->Add(FIREBALL_BOUNCE_SPRITE_4, 30, 8, 37, 15, tex);

	Animation* anim = new Animation(100);
	anim->Add(FIREBALL_BOUNCE_SPRITE_1);
	anim->Add(FIREBALL_BOUNCE_SPRITE_2);
	anim->Add(FIREBALL_BOUNCE_SPRITE_3);
	anim->Add(FIREBALL_BOUNCE_SPRITE_4);
	anims->Add(FIREBALL_BOUNCE_ANIM_ID, anim);

	// COLLISION

	sprites->Add(FIREBALL_COLLIDE_SPRITE_1, 40, 0, 55, 15, tex);
	sprites->Add(FIREBALL_COLLIDE_SPRITE_2, 58, 0, 73, 15, tex);
	sprites->Add(FIREBALL_COLLIDE_SPRITE_3, 76, 0, 91, 15, tex);
	anim = new Animation(150);
	anim->Add(FIREBALL_COLLIDE_SPRITE_1);
	anim->Add(FIREBALL_COLLIDE_SPRITE_2);
	anim->Add(FIREBALL_COLLIDE_SPRITE_3);

	anims->Add(FIREBALL_COLLIDE_ANIM_ID, anim);

	this->position.x = x;
	this->position.y = y;
	isDeleted = false;
	state = FireballState::Bouncing;
	creationTime = GetTickCount64();
	this->isFacingRight = isFacingRight;
	velocity.x = this->isFacingRight ? FIREBALL_SPEED : -FIREBALL_SPEED;
}

void Fireball::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (GetTickCount64() - creationTime > FIREBALL_LIFETIME) {
		isExploded = true;
		isDeleted = true;
		return;
	}
	if (isDeleted)
		return;
	if (state == FireballState::Exploding) {
		if (GetTickCount64() - explodeStartTime > FIREBALL_EXPLOSION_TIME) {
			isExploded = true;
			isDeleted = true;
		}
		return;
	}
	auto g = Game::GetInstance();
	velocity.y += FIREBALL_GRAVITY * dt;
	float camX, camY;
	Game::GetInstance()->GetCamera()->GetPosition(camX, camY);
	if (position.x < camX || position.x > camX + g->GetBackBufferWidth() || position.y > g->GetBackBufferHeight()) {
		isExploded = true;
		isDeleted = true;
		return;
	}

	
	vector<GameObject*> filteredCoObjects;
	for (auto obj : coObjects)
	{
		if (dynamic_cast<Fireball*>(obj) == nullptr && dynamic_cast<Mario*>(obj) == nullptr)
		{
			filteredCoObjects.push_back(obj);
		}
	}
	Collision::GetInstance()->ProcessCollision(this, filteredCoObjects, ctx->tilemap, dt);
}

void Fireball::Render()
{
	if (isDeleted)
		return;
	auto g = Game::GetInstance();
	float renderX, renderY;
	g->GetCamera()
		->WorldToScreen(position.x, position.y, renderX, renderY);
	if (state == FireballState::Bouncing) {
		Animations::GetInstance()->Get(FIREBALL_BOUNCE_ANIM_ID)->Render(round(renderX), round(renderY), isFacingRight, false);
	}
	else if (state == FireballState::Exploding) {
		Animations::GetInstance()->Get(FIREBALL_COLLIDE_ANIM_ID)->Render(round(renderX), round(renderY), isFacingRight, false);
	}

}

Rect Fireball::GetBoundingBox()
{
	if (isDeleted)
		return Rect();
	RectF r;
	r.top = position.y;
	r.left = position.x;
	r.bottom = position.y + 7.0f;
	r.right = position.x + 7.0f;
	return r;
}

void Fireball::OnNoCollision(float dt)
{
	position += velocity * dt;
}

void Fireball::OnCollisionWith(CollisionEvent* e)
{
	if (state == FireballState::Exploding) return;

	if (e->IsTileCollision() && e->otherTile->IsBlocking()) {
		if (e->normalizedDir.y == -1)
			velocity.y = FIREBALL_BOUNCE_SPEED; // bounce up if hit the ground
		else if (e->normalizedDir.y == 1)
			velocity.y = 0; //bounce down if hit the ceiling
		else if (e->normalizedDir.x != 0)
			Explode(); // explode if hit a wall
	}
	if (e->IsObjectCollision())
	{
		// resolve object collision
		const auto goomba = dynamic_cast<Goomba*>(e->otherObject);

		if (goomba != nullptr)
		{
			if (goomba->GetState() == GoombaState::Dead)
				return;
			this->isExploded = true;
			goomba->SetState(GoombaState::Dead);
			AudioManager::GetInstance()->PlaySFX(GOOMBA_STOMP);
			Explode();
		}
	}
}
void Fireball::Explode() {
	state = FireballState::Exploding;
	velocity.x = 0;
	velocity.y = 0;
	explodeStartTime = GetTickCount64();
}
