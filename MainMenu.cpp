#include "MainMenu.h"

#include <string>

#include "AssetIDs.h"
#include "FontManager.h"
#include "Game.h"
#include "InputManager.h"

std::wstring MainMenu::GetSceneElementString(SceneElement el)
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
	constexpr int xOffset = 55;
	constexpr int yOffset = -30;

	buttonStartPosition = Vector2Int(
		g->GetBackBufferWidth() / 2 - xOffset,
		g->GetBackBufferHeight() / 2 - yOffset
	);
}

void MainMenu::Update(float dt)
{
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
			Game::GetInstance()->IndicateSceneSwitch(0);
		}
	}
}

void MainMenu::Render()
{
	const auto f = FontManager::GetInstance();
	const auto g = Game::GetInstance();
	auto config = FontDrawConfig(
		Rect::FromXYWH(0, 0, g->GetBackBufferWidth(), 100), 
		L"MARIO NES - SE102", D3DXCOLOR(1.0, 1.0, 1.0, 1.0), \
		Center | VerticalCenter
	);


	f->Draw(FONTS::STATS_FONT, config);


	for (int i = PlayButton; i <= QuitButton; i++)
	{
		constexpr float offset = 30.0f;

		const auto drawX = buttonStartPosition.x;
		const auto drawY = buttonStartPosition.y + offset * static_cast<float>(i);

		auto msg = GetSceneElementString(static_cast<SceneElement>(i));
		if (i == currentFocus)
		{
			msg.insert(0, L"<");
			msg.append(L">");
		}

		f->Draw(FONTS::STATS_FONT,
			Vector2(drawX, drawY),
			msg.c_str(),
			D3DXCOLOR(1.0, 1.0, 1.0, 1.0)
		);
	}


}

void MainMenu::Load()
{
}

void MainMenu::UnLoad()
{
}
