#include "LevelTextRender.h"
#include "Game.h"
#include "AssetIDs.h"
#include "FontManager.h"
#include "Color.h"

Rect LevelTextRender::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 16, 16);
}

void LevelTextRender::Render()
{
	const auto game = Game::GetInstance();
	const auto camera = game->GetCamera();

	for (const auto& text : texts)
	{
		bool inView = camera->IsInView(text.zone);
		if (!inView)
			continue;

		int drawId = WORLD_FONT;
		if (text.fontWeight == Bold && text.italic)
		{
			drawId = WORLD_FONT_BOLD_ITALIC;
		}
		else if (text.fontWeight == Bold)
		{
			drawId = WORLD_FONT_BOLD;
		}else if (text.italic)
		{
			drawId = WORLD_FONT_ITALIC;
		}

		float screenX, screenY;
		camera->WorldToScreen(text.zone.left, text.zone.top, screenX, screenY);
		const auto screenZone = Rect::FromXYWH(
			screenX,
			screenY,
			text.zone.right - text.zone.left,
			text.zone.bottom - text.zone.top
		);

		FontManager::GetInstance()->Draw(
			drawId,
			FontDrawConfig(
				screenZone,
				text.content.c_str(), 
				Colors::WHITE, 
				text.textFormat
			)
		);
	}
}

LevelTextRender::LevelTextRender(const vector<WorldTextData>& texts)
{
	this->texts = texts;
	renderIndex = 999;
}
