#pragma once
#include "Scene.h"
#include "Timer.h"
#include <string>
class LevelTransitionScene : public Scene
{
	static constexpr float DEFAULT_TRANSITION_DURATION = 2.0f;

	Optional<MarioPower> marioPower{};
	int targetLevelID = 0;
	int renderLevel = 0;
	float transitionDuration = DEFAULT_TRANSITION_DURATION;
	Timer transitionTimer{};

	std::wstring scoreText{};
	std::wstring coinText{};
	std::wstring worldText{};
	std::wstring lifeText{};

	static int ToLevelIndex(int sceneId);
	void RefreshTexts();
	void DrawTextWithShadow(int fontId, const Rect& bounds, const std::wstring& text, Color color, UINT format) const;
public:
	LevelTransitionScene() = default;
	explicit LevelTransitionScene(float transitionDuration);
	void SetTargetLevelID(int targetLevelId);
	void Update(float dt) override;
	void Render() override;
	void Load(const Optional<SceneSwitchContext>& ctx) override;
};

