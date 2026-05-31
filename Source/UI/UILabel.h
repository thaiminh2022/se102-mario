#pragma once
#include "UIElement.h"

class UILabel : public UIElement
{
protected:
	bool isVisible;
public:
	UILabel(Vector2 pos, Vector2 size, std::wstring text, int fontId, Color color = Colors::WHITE, UINT align = TextFormat::Left)
		: UIElement(pos, size, text, fontId, color, align), isVisible(true) {
	}
	void SetColor(Color newColor) {
		this->textColor = newColor;
	}
	void SetVisible(bool visible) {
		isVisible = visible;
	}
	void Render() override;
};

