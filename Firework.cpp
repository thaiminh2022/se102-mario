#include "Firework.h"

#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "AudioManager.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"
#include <cmath>
#include "StatManager.h"

constexpr float FIREWORK_LIFETIME = 0.45f;

void Firework::EnsureAssets()
{
	auto anims = Animations::GetInstance();
	if (anims->Contains(FIREBALL_COLLIDE_ANIM_ID))
		return;

	auto sprites = Sprites::GetInstance();
	auto tex = Textures::GetInstance()->Get(FIREBALL_TEX_ID);

	sprites->Add(FIREBALL_COLLIDE_SPRITE_1, 40, 0, 55, 15, tex);
	sprites->Add(FIREBALL_COLLIDE_SPRITE_2, 58, 0, 73, 15, tex);
	sprites->Add(FIREBALL_COLLIDE_SPRITE_3, 76, 0, 91, 15, tex);

	auto anim = new Animation(150);
	anim->Add(FIREBALL_COLLIDE_SPRITE_1);
	anim->Add(FIREBALL_COLLIDE_SPRITE_2);
	anim->Add(FIREBALL_COLLIDE_SPRITE_3);
	anims->Add(FIREBALL_COLLIDE_ANIM_ID, anim);
}

Firework::Firework(Vector2 position) : GameObject(position)
{
	isCollidable = false;
	isBlocking = false;
	lifeTimer = Timer(FIREWORK_LIFETIME);
	lifeTimer.Start();
	EnsureAssets();
	AudioManager::GetInstance()->PlaySFX(FIREWORKS);
	StatManager::GetInstance()->AddScore(500);
}

void Firework::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	lifeTimer.ProcessTimer(dt);
	if (lifeTimer.IsFinished())
	{
		isDeleted = true;
	}
}

void Firework::Render()
{
	if (isDeleted)
		return;

	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);
	Animations::GetInstance()->Get(FIREBALL_COLLIDE_ANIM_ID)->Render(round(renderX), round(renderY), false, false);
}

Rect Firework::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 0, 0);
}
