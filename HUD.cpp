#include "HUD.h"
#include "FontManager.h"
#include "AssetIDs.h"
#include "StatManager.h"
#include <sstream>
#include <iomanip>
#include <memory>

HUD* HUD::_instance = nullptr;
HUD::HUD() {
	Init();
}
HUD* HUD::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new HUD();
	}
	return _instance;
}
void HUD::Init() {

	Vector2 scoreSize = FontManager::GetInstance()->MeasureString(HUD_FONT, L"SCORE\n000000");
	Vector2 livesSize = FontManager::GetInstance()->MeasureString(HUD_FONT, L"LIVES\n000");
	Vector2 coinSize = FontManager::GetInstance()->MeasureString(HUD_FONT, L"COINS\n000");
	Vector2 timerSize = FontManager::GetInstance()->MeasureString(HUD_FONT, L"TIME\n000");
	auto score = std::make_unique<UILabel>(Vector2(10, 8), scoreSize, L"SCORE\n000000", HUD_FONT, Colors::WHITE, TextFormat::Left | TextFormat::Top);
	auto coin = std::make_unique<UILabel>(Vector2(95, 8), coinSize, L"COINS\n0", HUD_FONT, Colors::WHITE, TextFormat::Left | TextFormat::Top);
	auto life = std::make_unique<UILabel>(Vector2(180, 8), livesSize, L"LIVES\n3", HUD_FONT, Colors::WHITE, TextFormat::Left | TextFormat::Top);
	auto time = std::make_unique<UILabel>(Vector2(265, 8), timerSize, L"TIME\n160", HUD_FONT, Colors::WHITE, TextFormat::Left | TextFormat::Top);

	scoreLabel = score.get();
	coinLabel = coin.get();
	lifeLabel = life.get();
	timeLabel = time.get();

	elements.push_back(std::move(score));
	elements.push_back(std::move(coin));
	elements.push_back(std::move(life));
	elements.push_back(std::move(time));
}
void HUD::Update(float dt) {
	auto statManager = StatManager::GetInstance();

	std::wstringstream ss;
	ss << std::setfill(L'0') << std::setw(6) << statManager->GetScore();
	std::wstring scoreText = (L"MARIO\n" + ss.str());
	scoreLabel->SetText(scoreText);

	std::wstring coinText = (L"COINS\n" + std::to_wstring(statManager->GetCoin()));
	coinLabel->SetText(coinText);

	std::wstring lifeText = (L"LIVES\n" + std::to_wstring(statManager->GetLife()));
	lifeLabel->SetText(lifeText);

}
void HUD::Render() {
	for (const auto& el : elements) {
		el->Render();
	}
}
HUD::~HUD() {
	elements.clear();
}

void HUD::Hide()
{
	for (const auto& el : elements) {
		if (auto label = dynamic_cast<UILabel*>(el.get())) {
			label->SetVisible(false);
		}
	}
}
