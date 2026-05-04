#pragma once
#include "UIElement.h"
#include <vector>
#include "UILabel.h"
class HUD
{
private:
	static HUD* _instance;
	std::vector<UIElement*> elements;
	UILabel* scoreLabel;
	UILabel* coinLabel;
	UILabel* lifeLabel;
	UILabel* timeLabel;
	HUD();

public:
	static HUD* GetInstance();
	void Update(float dt);
	void Render();
	void Init();
	~HUD();
	void Hide();
	UIElement* GetElement(int index) {
		if (index < 0 || index >= elements.size()) return nullptr;
		return elements[index];
	}
};

