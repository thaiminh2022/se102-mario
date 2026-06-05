#pragma once
#include "Scene.h"

class GameOverScene : public Scene
{
	bool showRestart = false;
	float elapsedTime = 0.0f;

	void DrawTitle() const;
	void DrawStats() const;
	void DrawScenery() const;

public:
	void Update(float dt) override;
	void Render() override;
	void Load(const Optional<SceneSwitchContext>& ctx) override;
	void UnLoad() override;
};
