#include "PointPopup.h"

#include "FontManager.h"
#include "Game.h"
#include "AssetIDs.h"

PointPopup::PointPopup(Vector2 pos, int scoreValue) : GameObject(pos)
{
	text = std::to_wstring(scoreValue);
	lifeTimer = Timer(0.6f);
	lifeTimer.Start();
	riseSpeed = 30.0f;
	isCollidable = false;
	isBlocking = false;
}

void PointPopup::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	position.y -= riseSpeed * dt;
	lifeTimer.ProcessTimer(dt);

	if (lifeTimer.IsFinished())
	{
		isDeleted = true;
	}
}

void PointPopup::Render()
{
	float renderX, renderY;
	Game::GetInstance()->GetCamera()->WorldToScreen(position.x, position.y, renderX, renderY);

	FontManager::GetInstance()->Draw(
		STATS_FONT,
		Vector2(renderX, renderY),
		text.c_str(),
		Colors::WHITE
	);
}

Rect PointPopup::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 0, 0);
}
