#pragma once
#include "Scene.h"
#include "Timer.h"
#include <string>
#include <vector>
class LevelTransitionScene : public Scene
{
	std::vector <std::wstring> Texts = {
		L"MARIO\n12345",
		L"x21",
		L"WORLD\n1-2",
		L"TIME"
	};

	Optional<MarioPower> marioPower;
	int targetLevelID;
	float transitionDuration;
	Timer transitionTimer;
public:
	LevelTransitionScene();
	LevelTransitionScene(float transitionDuration);
	void SetTargetLevelID(int targetLevelId);
	void Update(float dt) override;
	void Render() override;
	void Load(const Optional<SceneSwitchContext>& ctx) override;
};

