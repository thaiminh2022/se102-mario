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
	Color WithAlpha(const float newAlpha)
	{
		a = newAlpha;
		return *this;
	}
	
};

struct Colors
{
	constexpr Color White{ 1.f, 1.f, 1.f, 1.f };
	constexpr Color Black{ 0.f, 0.f, 0.f, 1.f };
	constexpr Color Red{ 1.f, 0.f, 0.f, 1.f };
	constexpr Color Green{ 0.f, 1.f, 0.f, 1.f };
	constexpr Color Blue{ 0.f, 0.f, 1.f, 1.f };
	constexpr Color Yellow{ 1.f, 1.f, 0.f, 1.f };
	constexpr Color Cyan{ 0.f, 1.f, 1.f, 1.f };
	constexpr Color Magenta{ 1.f, 0.f, 1.f, 1.f };
	constexpr Color Gray{ 0.5f, 0.5f, 0.5f, 1.f };
	constexpr Color LightGray{ 0.75f, 0.75f, 0.75f, 1.f };
	constexpr Color DarkGray{ 0.25f, 0.25f, 0.25f, 1.f };
	constexpr Color Orange{ 1.f, 0.5f, 0.f, 1.f };
	constexpr Color Purple{ 0.5f, 0.f, 0.5f, 1.f };
	constexpr Color Pink{ 1.f, 0.75f, 0.8f, 1.f };
	constexpr Color Transparent{ 0.f, 0.f, 0.f, 0.f };
};

