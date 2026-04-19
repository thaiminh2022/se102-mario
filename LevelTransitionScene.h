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

private:
	int targetLevelID;
	float transitionDuration;
	Timer transitionTimer;
public:
	LevelTransitionScene();
	LevelTransitionScene(int targetLevelID, float transitionDuration);
	void SetTargetLevelID(int targetLevelID);
	void Update(float dt) override;
	void Render() override;
	void Load() override;
	void UnLoad() override;

};

