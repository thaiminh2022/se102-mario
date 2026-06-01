#include "LevelTransitionScene.h"
#include "AssetIDs.h"
#include "Game.h"
#include "Sprite.h"
#include "Textures.h"
#include "Sprites.h"
#include "Color.h"
#include "FontManager.h"
#include "Mario.h"
#include "StatManager.h"

namespace
{
	constexpr Color ACCENT_RED{ 236.0f / 255.0f, 72.0f / 255.0f, 48.0f / 255.0f, 1.0f };
	constexpr Color ACCENT_GOLD{ 1.0f, 216.0f / 255.0f, 72.0f / 255.0f, 1.0f };
	constexpr Color SHADOW{ 28.0f / 255.0f, 28.0f / 255.0f, 28.0f / 255.0f, 1.0f };
	constexpr float COIN_X = 124.0f;
	constexpr float COIN_Y = 17.0f;
	constexpr float MARIO_X = 119.0f;
	constexpr float MARIO_Y = 143.0f;
}

LevelTransitionScene::LevelTransitionScene(float transitionDuration)
	: transitionDuration(transitionDuration)
{
}

void LevelTransitionScene::SetTargetLevelID(int targetLevelId)
{
	this->targetLevelID = targetLevelId;
	renderLevel = ToLevelIndex(targetLevelId);
	StatManager::GetInstance()->SetLevel(renderLevel);
}

int LevelTransitionScene::ToLevelIndex(int sceneId)
{
	return sceneId <= 0 ? -sceneId : sceneId;
}

void LevelTransitionScene::RefreshTexts()
{
	const auto statManager = StatManager::GetInstance();
	scoreText = L"MARIO\n" + std::to_wstring(statManager->GetScore());
	coinText = L"x " + std::to_wstring(statManager->GetCoin());
	worldText = L"WORLD\n1-" + std::to_wstring(renderLevel + 1);
	lifeText = L"x " + std::to_wstring(statManager->GetLife());
}

void LevelTransitionScene::DrawTextWithShadow(
	const int fontId,
	const Rect& bounds,
	const std::wstring& text,
	const Color color,
	const UINT format) const
{
	const auto f = FontManager::GetInstance();
	f->Draw(fontId, FontDrawConfig(bounds.Offset(1, 1), text.c_str(), SHADOW, format));
	f->Draw(fontId, FontDrawConfig(bounds, text.c_str(), color, format));
}

void LevelTransitionScene::Update(float dt)
{
	transitionTimer.ProcessTimer(dt);

	if (transitionTimer.IsFinished())
	{
		const auto marioPowerValue = marioPower.has_value() ? marioPower.value() : static_cast<MarioPower>(0);

		Game::GetInstance()
		->IndicateSceneSwitch(targetLevelID, SceneSwitchContext::NoTransition(marioPowerValue));
	}
}

void LevelTransitionScene::Render()
{
	const auto g = Game::GetInstance();
	const auto sprites = Sprites::GetInstance();

	float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	g->GetDirect3DDevice()->ClearRenderTargetView(g->GetRenderTargetView(), black);

	DrawTextWithShadow(
		FONTS::HUD_FONT,
		Rect::FromXYWH(14, 10, 96, 36),
		scoreText,
		Colors::WHITE,
		Left | Top
	);

	sprites->Get(HUDCOIN_SPRITE_ID)->Draw(COIN_X, COIN_Y, false, false);
	DrawTextWithShadow(
		FONTS::HUD_FONT,
		Rect::FromXYWH(144, 10, 56, 36),
		coinText,
		Colors::WHITE,
		Left | VerticalCenter
	);

	DrawTextWithShadow(
		FONTS::HUD_FONT,
		Rect::FromXYWH(230, 10, 76, 36),
		L"TIME",
		Colors::WHITE,
		Center | Top
	);

	DrawTextWithShadow(
		FONTS::STATS_FONT,
		Rect::FromXYWH(0, 76, g->GetBackBufferWidth(), 44),
		worldText,
		ACCENT_GOLD,
		Center | VerticalCenter
	);

	sprites->Get(HUDMARIO_SPRITE_ID)->Draw(MARIO_X, MARIO_Y, false, false);
	DrawTextWithShadow(
		FONTS::STATS_FONT,
		Rect::FromXYWH(148, 141, 70, 24),
		lifeText,
		ACCENT_RED,
		Left | VerticalCenter
	);
}

void LevelTransitionScene::Load(const Optional<SceneSwitchContext>& ctx)
{
	RefreshTexts();

	auto sprites = Sprites::GetInstance();
	sprites->Add(HUDCOIN_SPRITE_ID, 0, 0, 15, 15, Textures::GetInstance()->Get(HUDCOIN_TEX_ID));
	sprites->Add(HUDMARIO_SPRITE_ID, 0, 0, 15, 15, Textures::GetInstance()->Get(MARIO_TEX_ID));
	
	transitionTimer = Timer(transitionDuration);
	transitionTimer.Start();

	if (ctx.has_value())
	{
		marioPower = ctx.value().marioPower;
	}else
	{
		marioPower = {};
	}
}
