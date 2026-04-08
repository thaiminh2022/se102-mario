#pragma once
#include <d3dx10math.h>
#include "Rect.h"
#include <unordered_map>

#include "Vector2.h"

using std::unordered_map;

enum class FontWeight : std::uint16_t
{
	Normal = FW_NORMAL,
	Bold =  FW_BOLD,
	Black = FW_BLACK,
	Light = FW_LIGHT
};

enum TextFormat : UINT
{
	Left = DT_LEFT,
	Center = DT_CENTER,
	Right = DT_RIGHT,
	NoClip = DT_NOCLIP,
	Bottom = DT_BOTTOM,
	CalcRect = DT_CALCRECT,
	ExpandTab = DT_EXPANDTABS,
	RTLReading = DT_RTLREADING,
	SingleLine = DT_SINGLELINE,
	Top = DT_TOP,
	VerticalCenter = DT_VCENTER,
	WordBreak = DT_WORDBREAK
};

struct FontData
{
	int width, height;
	UINT weight;
	bool italic;
};

struct FontDrawConfig
{
	Rect r;
	LPCWSTR message;
	D3DXCOLOR color;
	UINT format;
};

/// A class to manage and draw font
/// As references to:
/// https://learn.microsoft.com/en-us/windows/win32/direct3d9/id3dxfont--drawtext
class FontManager
{
	static FontManager* _instance;
	unordered_map<int, ID3DX10Font*> fonts;

public:
	static  FontManager* GetInstance()
	{
		if (_instance == nullptr)
			_instance = new FontManager;

		return _instance;
	}

	void AddFontLocal(int id, LPCWSTR faceName, FontData fontData);
	void AddFontCustom(int id, LPCWSTR filePath, LPCWSTR faceName, FontData fontData);
	void Draw(int id, const FontDrawConfig& config);
	void Draw(int id, const Vector2& pos, LPCWSTR message, D3DXCOLOR color);
};

