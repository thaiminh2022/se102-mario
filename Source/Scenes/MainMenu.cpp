#include "MainMenu.h"

#include <string>

#include "AssetIDs.h"
#include "FontManager.h"
#include "Game.h"
#include "InputManager.h"
#include "Sprites.h"
#include "Textures.h"

namespace
{
	constexpr int MENU_MARIO_SPRITE = 910001;
	constexpr int MENU_BRICK_SPRITE = 910002;
	constexpr int MENU_QUESTION_SPRITE = 910003;
	constexpr int MENU_COIN_SPRITE = 910004;
	constexpr int MENU_MUSHROOM_SPRITE = 910005;
	constexpr int MENU_STAR_SPRITE = 910006;

	constexpr Color SKY_BLUE{ 92.0f / 255.0f, 148.0f / 255.0f, 252.0f / 255.0f, 1.0f };
	constexpr Color TITLE_RED{ 236.0f / 255.0f, 72.0f / 255.0f, 48.0f / 255.0f, 1.0f };
	constexpr Color TITLE_GOLD{ 1.0f, 216.0f / 255.0f, 72.0f / 255.0f, 1.0f };
	constexpr Color MENU_SHADOW{ 36.0f / 255.0f, 40.0f / 255.0f, 84.0f / 255.0f, 1.0f };
	constexpr Color MUTED_WHITE{ 232.0f / 255.0f, 240.0f / 255.0f, 1.0f, 1.0f };
}

std::wstring MainMenu::GetSceneElementString(SceneElement el) const
{
	switch (el)
	{
	case PlayButton:
		return L"Play";
	case AboutButton:
		return L"About";
	case QuitButton:
		return L"Quit";
	}

	return L"unknown button";
}

MainMenu::MainMenu()
{
	currentFocus = PlayButton;

	const auto g = Game::GetInstance();

	buttonStartPosition = Vector2Int(
		g->GetBackBufferWidth() / 2 - 50,
		g->GetBackBufferHeight() / 2 + 12
	);
}

void MainMenu::Update(float dt)
{
	elapsedTime += dt;
	auto input = InputManager::GetInstance();

	if (input->IsKeyDownThisFrame(VK_UP))
	{
		if (currentFocus == PlayButton)
		{
			currentFocus = QuitButton;
		}else
		{
			currentFocus = static_cast<SceneElement>(currentFocus - 1);
		}
	}else if (input->IsKeyDownThisFrame(VK_DOWN))
	{
		if (currentFocus == QuitButton)
		{
			currentFocus = PlayButton;
		}
		else
		{
			currentFocus = static_cast<SceneElement>(currentFocus + 1);
		}
	}else if (input->IsKeyDown(VK_RETURN)) // enter key
	{
		if (currentFocus == PlayButton)
		{
			// 0 is first playable level
			Game::GetInstance()->IndicateSceneSwitch(0, SceneSwitchContext::NormalTransition());
		}
	}
}

void MainMenu::Render()
{
	DrawScenery();
	DrawTitle();
	DrawMenuOptions();
}

void MainMenu::DrawTitle() const
{
	const auto f = FontManager::GetInstance();
	const auto g = Game::GetInstance();
	const Rect titleBounds = Rect::FromXYWH(0, 27, g->GetBackBufferWidth(), 34);

	f->Draw(FONTS::STATS_FONT, FontDrawConfig(Rect::FromXYWH(3, 30, g->GetBackBufferWidth(), 34), L"MARIO NES", MENU_SHADOW, Center | VerticalCenter));
	f->Draw(FONTS::STATS_FONT, FontDrawConfig(Rect::FromXYWH(1, 28, g->GetBackBufferWidth(), 34), L"MARIO NES", Colors::WHITE, Center | VerticalCenter));
	f->Draw(FONTS::STATS_FONT, FontDrawConfig(titleBounds, L"MARIO NES", TITLE_RED, Center | VerticalCenter));
	f->Draw(FONTS::WORLD_FONT_BOLD, FontDrawConfig(Rect::FromXYWH(0, 58, g->GetBackBufferWidth(), 14), L"SE102 EDITION", TITLE_GOLD, Center | VerticalCenter));
}

void MainMenu::DrawScenery() const
{
	auto sprites = Sprites::GetInstance();
	const auto g = Game::GetInstance();
	const float bob = elapsedTime - static_cast<int>(elapsedTime);
	const float coinOffset = bob < 0.5f ? 0.0f : -2.0f;

	for (int x = 0; x < g->GetBackBufferWidth(); x += 16)
	{
		sprites->Get(MENU_BRICK_SPRITE)->Draw(static_cast<float>(x), 224.0f, false, false);
	}

	for (int i = 0; i < 6; ++i)
	{
		const float x = 48.0f + i * 16.0f;
		const bool useQuestion = i == 1 || i == 4;
		sprites->Get(useQuestion ? MENU_QUESTION_SPRITE : MENU_BRICK_SPRITE)->Draw(x, 86.0f, false, false);
	}

	sprites->Get(MENU_MARIO_SPRITE)->Draw(76.0f, 208.0f, false, false);
	sprites->Get(MENU_MUSHROOM_SPRITE)->Draw(224.0f, 208.0f, false, false);
	sprites->Get(MENU_STAR_SPRITE)->Draw(246.0f, 194.0f, false, false);
	sprites->Get(MENU_COIN_SPRITE)->Draw(83.0f, 66.0f + coinOffset, false, false);
	sprites->Get(MENU_COIN_SPRITE)->Draw(131.0f, 66.0f - coinOffset, false, false);
	sprites->Get(MENU_COIN_SPRITE)->Draw(246.0f, 88.0f + coinOffset, false, false);
}

void MainMenu::DrawMenuOptions() const
{
	const auto f = FontManager::GetInstance();
	const auto g = Game::GetInstance();
	const auto sprites = Sprites::GetInstance();

	f->Draw(WORLD_FONT_BOLD, FontDrawConfig(
		Rect::FromXYWH(0, 110, g->GetBackBufferWidth(), 14),
		L"ARROWS TO MOVE    ENTER TO START",
		MUTED_WHITE,
		Center | VerticalCenter
	));

	for (int i = PlayButton; i <= QuitButton; i++)
	{
		constexpr float offset = 26.0f;

		const auto drawX = buttonStartPosition.x;
		const auto drawY = buttonStartPosition.y + offset * static_cast<float>(i);
		const bool isFocused = i == currentFocus;
		const auto msg = GetSceneElementString(static_cast<SceneElement>(i));
		const Color textColor = isFocused ? TITLE_GOLD : Colors::WHITE;

		if (isFocused)
		{
			sprites->Get(MENU_COIN_SPRITE)->Draw(drawX - 23.0f, drawY + 1.0f, false, false);
			f->Draw(FONTS::STATS_FONT, Vector2(drawX - 2.0f, drawY + 2.0f), msg.c_str(), MENU_SHADOW);
		}

		f->Draw(FONTS::STATS_FONT, Vector2(drawX, drawY), msg.c_str(), textColor);
	}
}

void MainMenu::Load(const Optional<SceneSwitchContext>& ctx)
{
	auto sprites = Sprites::GetInstance();
	auto textures = Textures::GetInstance();

	sprites->Add(MENU_MARIO_SPRITE, 0, 0, 15, 15, textures->Get(MARIO_TEX_ID));
	sprites->Add(MENU_BRICK_SPRITE, 32, 0, 47, 15, textures->Get(BLOCKS_OVERWORLD_TEX_ID));
	sprites->Add(MENU_QUESTION_SPRITE, 0, 0, 15, 15, textures->Get(BLOCKS_OVERWORLD_TEX_ID));
	sprites->Add(MENU_COIN_SPRITE, 0, 0, 15, 15, textures->Get(OVERWORLD_ITEMS_TEX_ID));
	sprites->Add(MENU_MUSHROOM_SPRITE, 0, 16, 15, 31, textures->Get(OVERWORLD_ITEMS_TEX_ID));
	sprites->Add(MENU_STAR_SPRITE, 0, 32, 15, 47, textures->Get(OVERWORLD_ITEMS_TEX_ID));

	Game::GetInstance()->SetBackgroundColor(SKY_BLUE);
}

void MainMenu::UnLoad()
{
	Game::GetInstance()->SetBackgroundColor({});
}
