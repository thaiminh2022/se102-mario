#include "HUD.h"
#include "FontManager.h"
#include "AssetIDs.h"
#include "StatManager.h"
#include <sstream>
#include <iomanip>

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
	scoreLabel = new UILabel(Vector2(10, 8), scoreSize, L"SCORE\n000000", HUD_FONT, Colors::WHITE, TextFormat::Left | TextFormat::Top);
	coinLabel = new UILabel(Vector2(95, 8), coinSize, L"COINS\n0", HUD_FONT, Colors::WHITE, TextFormat::Left | TextFormat::Top);
	lifeLabel = new UILabel(Vector2(180, 8), livesSize, L"LIVES\n3", HUD_FONT, Colors::WHITE, TextFormat::Left | TextFormat::Top);
	timeLabel = new UILabel(Vector2(265, 8), timerSize, L"TIME\n160", HUD_FONT, Colors::WHITE, TextFormat::Left | TextFormat::Top);
	elements.push_back(scoreLabel);
	elements.push_back(coinLabel);
	elements.push_back(lifeLabel);
	elements.push_back(timeLabel);
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
	for (auto el : elements) {
		el->Render();
	}
}
HUD::~HUD() {
	for (auto el : elements) {
		delete el;
	}
	elements.clear();
}

void HUD::Hide()
{
	for (auto el : elements) {
		if (auto label = dynamic_cast<UILabel*>(el)) {
			label->SetVisible(false);
		}
	}
}
