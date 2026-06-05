#include "AboutScene.h"

#include "AssetIDs.h"
#include "FontManager.h"
#include "Game.h"
#include "InputManager.h"
#include "Sprites.h"
#include "Textures.h"

namespace
{
	constexpr int ABOUT_BRICK_SPRITE = 930001;
	constexpr int ABOUT_COIN_SPRITE = 930002;
	constexpr int ABOUT_MARIO_SPRITE = 930003;
	constexpr int ABOUT_QUESTION_SPRITE = 930004;

	constexpr Color ABOUT_SKY{ 92.0f / 255.0f, 148.0f / 255.0f, 252.0f / 255.0f, 1.0f };
	constexpr Color ABOUT_RED{ 236.0f / 255.0f, 72.0f / 255.0f, 48.0f / 255.0f, 1.0f };
	constexpr Color ABOUT_GOLD{ 1.0f, 216.0f / 255.0f, 72.0f / 255.0f, 1.0f };
	constexpr Color ABOUT_SHADOW{ 36.0f / 255.0f, 40.0f / 255.0f, 84.0f / 255.0f, 1.0f };
	constexpr Color ABOUT_MUTED{ 232.0f / 255.0f, 240.0f / 255.0f, 1.0f };
}

void AboutScene::Update(float dt)
{
	elapsedTime += dt;

	const auto input = InputManager::GetInstance();
	if (input->IsKeyDownThisFrame(VK_ESCAPE))
	{
		Game::GetInstance()->IndicateSceneSwitch(MAIN_MENU, {});
	}
}

void AboutScene::Render()
{
	DrawScenery();
	DrawText();
}

void AboutScene::DrawScenery() const
{
	const auto g = Game::GetInstance();
	const auto sprites = Sprites::GetInstance();
	const float coinBob = static_cast<int>(elapsedTime * 4.0f) % 2 == 0 ? 0.0f : -2.0f;

	for (int x = 0; x < g->GetBackBufferWidth(); x += 16)
	{
		sprites->Get(ABOUT_BRICK_SPRITE)->Draw(static_cast<float>(x), 224.0f, false, false);
	}

	sprites->Get(ABOUT_MARIO_SPRITE)->Draw(48.0f, 208.0f, false, false);
	sprites->Get(ABOUT_QUESTION_SPRITE)->Draw(256.0f, 160.0f, false, false);
	sprites->Get(ABOUT_COIN_SPRITE)->Draw(260.0f, 138.0f + coinBob, false, false);
}

void AboutScene::DrawText() const
{
	const auto g = Game::GetInstance();
	const auto f = FontManager::GetInstance();

	const Rect titleRect = Rect::FromXYWH(0, 28, g->GetBackBufferWidth(), 30);
	f->Draw(STATS_FONT, FontDrawConfig(titleRect.Offset(3, 3), L"ABOUT", ABOUT_SHADOW, Center | VerticalCenter));
	f->Draw(STATS_FONT, FontDrawConfig(titleRect.Offset(1, 1), L"ABOUT", Colors::WHITE, Center | VerticalCenter));
	f->Draw(STATS_FONT, FontDrawConfig(titleRect, L"ABOUT", ABOUT_RED, Center | VerticalCenter));

	f->Draw(WORLD_FONT_BOLD, FontDrawConfig(Rect::FromXYWH(0, 66, g->GetBackBufferWidth(), 14), L"SE102 MARIO", ABOUT_GOLD, Center | VerticalCenter));
	f->Draw(WORLD_FONT, FontDrawConfig(Rect::FromXYWH(22, 94, 276, 18), L"A SUPER MARIO BROS STYLE PROJECT", Colors::WHITE, Center | VerticalCenter));
	f->Draw(WORLD_FONT, FontDrawConfig(Rect::FromXYWH(22, 118, 276, 18), L"BUILT WITH C++20 AND DIRECTX", ABOUT_MUTED, Center | VerticalCenter));
	f->Draw(WORLD_FONT, FontDrawConfig(Rect::FromXYWH(22, 142, 276, 18), L"ARROWS MOVE   W JUMPS   SHIFT RUNS", Colors::WHITE, Center | VerticalCenter));

	const bool showPrompt = static_cast<int>(elapsedTime * 3.0f) % 2 == 0;
	if (showPrompt)
	{
		f->Draw(HUD_FONT, FontDrawConfig(Rect::FromXYWH(0, 188, g->GetBackBufferWidth(), 14), L"ESC TO RETURN", Colors::WHITE, Center | VerticalCenter));
	}
}

void AboutScene::Load(const Optional<SceneSwitchContext>& ctx)
{
	elapsedTime = 0.0f;

	const auto sprites = Sprites::GetInstance();
	const auto textures = Textures::GetInstance();

	sprites->Add(ABOUT_BRICK_SPRITE, 32, 0, 47, 15, textures->Get(BLOCKS_OVERWORLD_TEX_ID));
	sprites->Add(ABOUT_COIN_SPRITE, 0, 0, 15, 15, textures->Get(OVERWORLD_ITEMS_TEX_ID));
	sprites->Add(ABOUT_MARIO_SPRITE, 0, 0, 15, 15, textures->Get(MARIO_TEX_ID));
	sprites->Add(ABOUT_QUESTION_SPRITE, 0, 0, 15, 15, textures->Get(BLOCKS_OVERWORLD_TEX_ID));

	Game::GetInstance()->SetBackgroundColor(ABOUT_SKY);
}

void AboutScene::UnLoad()
{
	Game::GetInstance()->SetBackgroundColor({});
}
