#pragma once
#include "Vector2.h"
#include <string>
#include "Color.h"
#include "FontManager.h"
class UIElement
{
protected:
	Vector2 position;
	Vector2 size;
	std::wstring text;
	int fontId;
	Color textColor;
	UINT alignment;
public:
	UIElement(Vector2 pos, Vector2 size, std::wstring text, int fontId, Color color = Colors::WHITE, UINT align = TextFormat::Left);
	virtual ~UIElement() = default;
	virtual void Update(float dt) {}
	virtual void Render();
	void SetText(std::wstring newText) { text = newText; }
	void SetPosition(Vector2 pos) { position = pos; }
	void SetSize(Vector2 newSize) { size = newSize; }
	void SetColor(Color newColor) { textColor = newColor; }
	void SetAlignment(UINT align) { alignment = align; }

};

