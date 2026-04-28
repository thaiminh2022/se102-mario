#include "UIElement.h"
#include "FontManager.h"

UIElement::UIElement(Vector2 pos, Vector2 size, std::wstring text, int fontId, Color color, UINT align)
	: position(pos), size(size), text(text), fontId(fontId), textColor(color), alignment(align)
{
    alignment = TextFormat::Center | TextFormat::VerticalCenter; // Default
}
void UIElement::Render() {
    auto f = FontManager::GetInstance();

    FontDrawConfig config(
        Rect::FromXYWH(position.x, position.y, size.x, size.y),
        text.c_str(),
        textColor,
        alignment
    );

    f->Draw(fontId, config);
}