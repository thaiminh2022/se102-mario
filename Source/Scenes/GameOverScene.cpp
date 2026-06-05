#include "GameOverScene.h"

#include "AssetIDs.h"
#include "AudioManager.h"
#include "FontManager.h"
#include "Game.h"
#include "InputManager.h"
#include "Sprites.h"
#include "StatManager.h"
#include "Textures.h"

#include <string>

namespace
{
	constexpr int GAME_OVER_MARIO_SPRITE = 920001;
	constexpr int GAME_OVER_BRICK_SPRITE = 920002;
	constexpr int GAME_OVER_COIN_SPRITE = 920003;

	constexpr Color GAME_OVER_BLACK{ 0.0f, 0.0f, 0.0f, 1.0f };
	constexpr Color GAME_OVER_RED{ 224.0f / 255.0f, 56.0f / 255.0f, 40.0f / 255.0f, 1.0f };
	constexpr Color GAME_OVER_GOLD{ 1.0f, 208.0f / 255.0f, 72.0f / 255.0f, 1.0f };
	constexpr Color GAME_OVER_SHADOW{ 64.0f / 255.0f, 32.0f / 255.0f, 32.0f / 255.0f, 1.0f };
	constexpr Color GAME_OVER_MUTED{ 184.0f / 255.0f, 184.0f / 255.0f, 184.0f / 255.0f, 1.0f };
}

void GameOverScene::Update(float dt)
{
	elapsedTime += dt;

	if (elapsedTime > 1.25f)
	{
		showRestart = true;
	}

	if (showRestart && InputManager::GetInstance()->IsKeyDownThisFrame(VK_RETURN))
	{
		AudioManager::GetInstance()->StopAll();
		StatManager::GetInstance()->Reset();
		Game::GetInstance()->IndicateSceneSwitch(0, SceneSwitchContext::NormalTransition());
	}
}

void GameOverScene::Render()
{
	DrawScenery();
	DrawTitle();
	DrawStats();
}

void GameOverScene::DrawTitle() const
{
	auto g = Game::GetInstance();
	auto f = FontManager::GetInstance();
	const Rect titleRect = Rect::FromXYWH(0, 48, g->GetBackBufferWidth(), 34);

	f->Draw(STATS_FONT, FontDrawConfig(titleRect.Offset(3, 3), L"GAME OVER", GAME_OVER_SHADOW, Center | VerticalCenter));
	f->Draw(STATS_FONT, FontDrawConfig(titleRect.Offset(1, 1), L"GAME OVER", Colors::WHITE, Center | VerticalCenter));
	f->Draw(STATS_FONT, FontDrawConfig(titleRect, L"GAME OVER", GAME_OVER_RED, Center | VerticalCenter));
	f->Draw(WORLD_FONT_BOLD, FontDrawConfig(Rect::FromXYWH(0, 78, g->GetBackBufferWidth(), 14), L"THE ADVENTURE ENDS HERE", GAME_OVER_GOLD, Center | VerticalCenter));
}

void GameOverScene::DrawStats() const
{
	auto g = Game::GetInstance();
	auto f = FontManager::GetInstance();
	auto stats = StatManager::GetInstance();

	const auto scoreText = L"SCORE " + std::to_wstring(stats->GetScore());
	const auto coinText = L"COINS " + std::to_wstring(stats->GetCoin());
	const auto worldText = L"WORLD " + std::to_wstring(stats->GetLevel() + 1);

	f->Draw(WORLD_FONT_BOLD, FontDrawConfig(Rect::FromXYWH(0, 112, g->GetBackBufferWidth(), 12), scoreText.c_str(), Colors::WHITE, Center | VerticalCenter));
	f->Draw(WORLD_FONT_BOLD, FontDrawConfig(Rect::FromXYWH(0, 130, g->GetBackBufferWidth(), 12), coinText.c_str(), GAME_OVER_GOLD, Center | VerticalCenter));
	f->Draw(WORLD_FONT_BOLD, FontDrawConfig(Rect::FromXYWH(0, 148, g->GetBackBufferWidth(), 12), worldText.c_str(), GAME_OVER_MUTED, Center | VerticalCenter));

	const bool showPrompt = showRestart && (static_cast<int>(elapsedTime * 3.0f) % 2 == 0);
	if (showPrompt)
	{
		f->Draw(HUD_FONT, FontDrawConfig(Rect::FromXYWH(0, 184, g->GetBackBufferWidth(), 16), L"PRESS ENTER TO TRY AGAIN", Colors::WHITE, Center | VerticalCenter));
	}
}

void GameOverScene::DrawScenery() const
{
	auto g = Game::GetInstance();
	auto sprites = Sprites::GetInstance();
	const float coinBob = (static_cast<int>(elapsedTime * 4.0f) % 2 == 0) ? 0.0f : -2.0f;

	for (int x = 0; x < g->GetBackBufferWidth(); x += 16)
	{
		sprites->Get(GAME_OVER_BRICK_SPRITE)->Draw(static_cast<float>(x), 224.0f, false, false);
	}

	sprites->Get(GAME_OVER_MARIO_SPRITE)->Draw(152.0f, 204.0f, false, false);
	sprites->Get(GAME_OVER_COIN_SPRITE)->Draw(72.0f, 104.0f + coinBob, false, false);
	sprites->Get(GAME_OVER_COIN_SPRITE)->Draw(232.0f, 104.0f - coinBob, false, false);
}

void GameOverScene::Load(const Optional<SceneSwitchContext>& ctx)
{
	showRestart = false;
	elapsedTime = 0.0f;

	auto sprites = Sprites::GetInstance();
	auto textures = Textures::GetInstance();

	sprites->Add(GAME_OVER_MARIO_SPRITE, 96, 0, 111, 15, textures->Get(MARIO_TEX_ID));
	sprites->Add(GAME_OVER_BRICK_SPRITE, 32, 0, 47, 15, textures->Get(BLOCKS_OVERWORLD_TEX_ID));
	sprites->Add(GAME_OVER_COIN_SPRITE, 0, 0, 15, 15, textures->Get(OVERWORLD_ITEMS_TEX_ID));

	Game::GetInstance()->SetBackgroundColor(GAME_OVER_BLACK);
	AudioManager::GetInstance()->StopAll();
	AudioManager::GetInstance()->PlayMusic(GAMEOVER, false, [&member = showRestart]
	{
			member = true;
	});
}

void GameOverScene::UnLoad()
{
	Game::GetInstance()->SetBackgroundColor({});
}
