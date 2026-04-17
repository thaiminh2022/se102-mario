#pragma once
#include "d3dx10math.h"

// streamline color behavior
struct Color
{
	float r, g, b, a;

	constexpr Color(float r, float g, float b, float a = 1.0f)
		: r(r), g(g), b(b), a(a) {}

	explicit Color (const unsigned int hex)
	{
		r = (hex >> 24 & 0xFF) / 255.0f;
		g = (hex >> 16 & 0xFF) / 255.0f;
		b = (hex >> 8 & 0xFF) / 255.0f;
		a = (0xFF & hex) / 255.0f;
	}

	D3DXCOLOR GetD3DXColor() const
	{
		return D3DXCOLOR(r, g, b, a);
	}
	Color WithAlpha(const float newAlpha) const
	{
		return Color{
			r,
			g,
			b,
			newAlpha
		};
	}
	
};

struct Colors
{
	static constexpr Color WHITE{ 1.f, 1.f, 1.f, 1.f };
	static constexpr Color BLACK{0.f, 0.f, 0.f, 1.f};
	static constexpr Color RED{1.f, 0.f, 0.f, 1.f};
	static constexpr Color GREEN{0.f, 1.f, 0.f, 1.f};
	static constexpr Color BLUE{0.f, 0.f, 1.f, 1.f};
	static constexpr Color YELLOW{1.f, 1.f, 0.f, 1.f};
	static constexpr Color CYAN{0.f, 1.f, 1.f, 1.f};
	static constexpr Color MAGENTA{1.f, 0.f, 1.f, 1.f};
	static constexpr Color GRAY{0.5f, 0.5f, 0.5f, 1.f};
	static constexpr Color LIGHT_GRAY{0.75f, 0.75f, 0.75f, 1.f};
	static constexpr Color DARK_GRAY{0.25f, 0.25f, 0.25f, 1.f};
	static constexpr Color ORANGE{1.f, 0.5f, 0.f, 1.f};
	static constexpr Color PURPLE{0.5f, 0.f, 0.5f, 1.f};
	static constexpr Color PINK{1.f, 0.75f, 0.8f, 1.f};
	static constexpr Color TRANSPARENT_COLOR { 0.f, 0.f, 0.f, 0.f };
};

