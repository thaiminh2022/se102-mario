#include "LevelTextRender.h"

#include "Game.h"
#include "AssetIDs.h"

Rect LevelTextRender::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 16, 16);
}

void LevelTextRender::Render()
{
	
	for (auto text : texts)
	{
		bool inView = Game::GetInstance()->GetCamera()->IsInView(text.zone);
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

		FontManager::GetInstance()->Draw(
			drawId,
			FontDrawConfig(
				text.zone, 
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
