#include "FireballTrap.h"

#include <algorithm>

#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"

FireballTrap::FireballTrap(Vector2 startPosition) : GameObject(startPosition.x, startPosition.y)
{
	auto sprites = Sprites::GetInstance();
	auto anims = Animations::GetInstance();
	auto tex = Textures::GetInstance()->Get(FIREBALL_TEX_ID);

	// fireball idling is basically fireball bouncing
	if (!anims->Contains(FIREBALL_BOUNCE_ANIM_ID))
	{
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
	}
	currentAngle = 0;

	// center
	auto rect = FireballTrap::GetBoundingBox();
	center = Vector2(
		static_cast<float>(rect.GetWidth()) / 2.0f + static_cast<float>(rect.left), 
		static_cast<float>(rect.GetHeight()) / 2.0f + static_cast<float>(rect.top)
	);

	// layout the left
	for (auto i =0; i < MAX_FIREBALL_TRAP_COUNT; i++)
	{
		fireballPositions.emplace_back(
			center.x * i,
			center.y
		);
	}

}

void FireballTrap::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	// move it in a circle fashion?
	currentAngle -= dt * 100;
	if (abs(currentAngle) >= 360)
	{
		currentAngle = 0;
	}

	for (auto i = 0; i < fireballPositions.size(); i++)
	{

		float dist = 10;
		float offset = 8;
		auto radian = currentAngle * 3.1415f / 180.0f;
		auto x = center.x + dist * i * cos(radian) - offset;
		auto y = center.y + dist * i * sin(radian) - offset;

		fireballPositions[i] = Vector2(x, y);
	}


}

void FireballTrap::Render()
{

	Game::GetInstance()->DrawDebugRectWithCamera(
		GetBoundingBox(),
		D3DXCOLOR(0.0, 1.0, 0.0, 0.4)
	);


	float renderX, renderY;
	auto cam = Game::GetInstance()->GetCamera();
	for (const auto& pos: fireballPositions)
	{
		cam->WorldToScreen(pos.x, pos.y, renderX, renderY);
		Animations::GetInstance()
		->Get(FIREBALL_BOUNCE_ANIM_ID)
		->Render(round(renderX), round(renderY), false, false);
	
		Game::GetInstance()->DrawDebugRectRaw(
			Rect::FromXYWH(renderX, renderY, 8, 8), 
			D3DXCOLOR(0.0, 1.0, 0.0, 1.0)
		);
	}
}

bool FireballTrap::IsHitSmallBalls(Rect checkRect) const
{

	return std::any_of(fireballPositions.begin(), fireballPositions.end(), 
	[&checkRect](const Vector2& p)
	{
		Rect r = Rect::FromXYWH(p.x, p.y, 8, 8);
		return r.IsColliding(checkRect);
	});
}
