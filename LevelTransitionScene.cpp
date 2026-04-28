#include "LevelTransitionScene.h"
#include "AssetIDs.h"
#include "Game.h"
#include "Sprite.h"
#include "Textures.h"
#include "Sprites.h"
#include "Color.h"
#include "FontManager.h"
#include "StatManager.h"

LevelTransitionScene::LevelTransitionScene()
{
	targetLevelID = 0;
	transitionDuration = 0.5f;
}

LevelTransitionScene::LevelTransitionScene(float transitionDuration)
{
	targetLevelID = 0;
	this->transitionDuration = transitionDuration;
}

void LevelTransitionScene::SetTargetLevelID(int targetLevelID)
{
	this->targetLevelID = targetLevelID;
}

void LevelTransitionScene::Update(float dt)
{
	transitionTimer.ProcessTimer(dt);

	if (transitionTimer.IsFinished())
	{
		Game::GetInstance()->IndicateSceneSwitch(targetLevelID, {});
	}
}

void LevelTransitionScene::Render()
{
	const auto f = FontManager::GetInstance();
	const auto g = Game::GetInstance();
	float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	g->GetDirect3DDevice()->ClearRenderTargetView(g->GetRenderTargetView(), black);// Clear screen to black

	for (int i = 0; i < Texts.size(); i++)
	{
		float x = i * 150;
		float y = 0;
		if (i == 1)
		{
			auto coinSprite = Sprites::GetInstance()->Get(HUDCOIN_SPRITE_ID);
			coinSprite->Draw(x + 48, y + 24, false, false);

			auto config = FontDrawConfig(
				Rect::FromXYWH(x, y, 32 * 6, 64),
				Texts[i].c_str(),
				Colors::WHITE,
				Center | VerticalCenter
			);
			f->Draw(FONTS::HUD_FONT, config);
		}
		else {
			auto config = FontDrawConfig(
				Rect::FromXYWH(x, y, 32 * 6, 64),
				Texts[i].c_str(),
				Colors::WHITE,
				Center | VerticalCenter
			);
			f->Draw(FONTS::HUD_FONT, config);
		}
	}
}

void LevelTransitionScene::Load(const Optional<SceneSwitchContext>& ctx)
{

	Texts[0] = L"MARIO\n" + std::to_wstring(StatManager::GetScore());
	Texts[1] = L"x " + std::to_wstring(StatManager::GetCoin());
	Texts[2] = L"WORLD\n1-" + std::to_wstring(targetLevelID + 1);
	auto sprites = Sprites::GetInstance();
	sprites->Add(HUDCOIN_SPRITE_ID, 0, 0, 15, 15, Textures::GetInstance()->Get(HUDCOIN_TEX_ID));
	
	transitionTimer = Timer(transitionDuration);
	transitionTimer.Start();
}
