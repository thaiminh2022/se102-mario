#include "FireballTrap.h"

#include <algorithm>

#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"

FireballTrap::FireballTrap(Vector2 startPosition)
{
	position = startPosition;
	const auto sprites = Sprites::GetInstance();
	const auto anims = Animations::GetInstance();
	const auto tex = Textures::GetInstance()->Get(FIREBALL_TEX_ID);

	// fireball idling is basically fireball bouncing
	if (!anims->Contains(FIREBALL_BOUNCE_ANIM_ID))
	{
		// BOUNCING
		sprites->Add(FIREBALL_BOUNCE_SPRITE_1, 0, 8, 7, 15, tex);
		sprites->Add(FIREBALL_BOUNCE_SPRITE_2, 10, 8, 17, 15, tex);
		sprites->Add(FIREBALL_BOUNCE_SPRITE_3, 20, 8, 27, 15, tex);
		sprites->Add(FIREBALL_BOUNCE_SPRITE_4, 30, 8, 37, 15, tex);

		auto anim = new Animation(100);
		anim->Add(FIREBALL_BOUNCE_SPRITE_1);
		anim->Add(FIREBALL_BOUNCE_SPRITE_2);
		anim->Add(FIREBALL_BOUNCE_SPRITE_3);
		anim->Add(FIREBALL_BOUNCE_SPRITE_4);
		anims->Add(FIREBALL_BOUNCE_ANIM_ID, anim);
	}
	currentAngle = 0;

	// center
	auto rect = FireballTrap::GetBoundingBox();
	position.x -= rect.GetWidth() / 2;
	position.y -= rect.GetHeight() / 2;
	
	
	center = Vector2(
		static_cast<float>(rect.GetWidth()) / 2.0f + position.x, 
		static_cast<float>(rect.GetHeight()) / 2.0f + position.y
	);

	// default positions start on the left side

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
		constexpr float dist = 10;
		constexpr float offset = 4;
		const auto radian = currentAngle * 3.1415f / 180.0f;
		const auto x = center.x + dist * i * cos(radian) + offset;
		const auto y = center.y + dist * i * sin(radian) + offset;

		fireballPositions[i].x = x;
		fireballPositions[i].y = y;
	}


}

void FireballTrap::Render()
{

	Game::GetInstance()->DrawDebugRectWithCamera(
		GetBoundingBox(),
		Colors::GREEN.WithAlpha(0.5f)
	);


	float renderX, renderY;
	const auto cam = Game::GetInstance()->GetCamera();
	for (const auto& pos: fireballPositions)
	{
		cam->WorldToScreen(pos.x, pos.y, renderX, renderY);
		Animations::GetInstance()
		->Get(FIREBALL_BOUNCE_ANIM_ID)
		->Render(round(renderX), round(renderY), false, false);
	
		
	}
}

bool FireballTrap::IsHitSmallBalls(Rect checkRect) const
{

	return std::any_of(fireballPositions.begin(), fireballPositions.end(), 
	[&checkRect](const Vector2& p)
	{
		const Rect r = Rect::FromXYWH(p.x, p.y, 8, 8);
		return r.IsColliding(checkRect);
	});
}
