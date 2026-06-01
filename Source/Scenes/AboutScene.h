#pragma once
#include "Scene.h"

class AboutScene : public Scene
{
	float elapsedTime = 0.0f;

	void DrawScenery() const;
	void DrawText() const;

public:
	void Update(float dt) override;
	void Render() override;
	void Load(const Optional<SceneSwitchContext>& ctx) override;
	void UnLoad() override;
};
