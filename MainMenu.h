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
	std::wstring GetSceneElementString(SceneElement el);

public:
	MainMenu();
	void Update(float dt) override;
	void Render() override;
	void Load(const Optional<SceneSwitchContext>& ctx) override;
	void UnLoad() override;
};

