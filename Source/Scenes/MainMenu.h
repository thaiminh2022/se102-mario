#pragma once
#include "Scene.h"
#include <cinttypes>
#include <string>

class MainMenu : public Scene
{
	enum SceneElement : std::uint8_t
	{
		PlayButton,
		AboutButton,
		QuitButton,
	};

	Vector2 buttonStartPosition;
	SceneElement currentFocus;
	float elapsedTime = 0.0f;
	std::wstring GetSceneElementString(SceneElement el) const;
	void DrawTitle() const;
	void DrawScenery() const;
	void DrawMenuOptions() const;

public:
	MainMenu();
	void Update(float dt) override;
	void Render() override;
	void Load(const Optional<SceneSwitchContext>& ctx) override;
	void UnLoad() override;
};

