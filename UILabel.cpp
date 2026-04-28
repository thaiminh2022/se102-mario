#include "UILabel.h"
#include "FontManager.h"

void UILabel::Render() {
    if (!isVisible) return;

    auto f = FontManager::GetInstance();

    FontDrawConfig config(
        Rect::FromXYWH(position.x, position.y, size.x, size.y),
        text.c_str(),
        textColor,
        alignment
    );

    f->Draw(fontId, config);
}