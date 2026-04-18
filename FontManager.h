#pragma once
#include <d3dx10math.h>
#include "Rect.h"
#include <unordered_map>

#include "Color.h"
#include "Vector2.h"

using std::unordered_map;

enum FontWeight : UINT16
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

	FontData(int height, UINT weight = FontWeight::Normal, bool italic = false)
		: width(0),
		  height(height),
		  weight(weight),
		  italic(italic)
	{
	}
};

struct FontDrawConfig
{
	Rect r;
	LPCWSTR message;
	Color color;
	UINT format;

	FontDrawConfig(const Rect& r, const LPCWSTR message, const Color& color, const UINT format)
		: r(r),
		  message(message),
		  color(color),
		  format(format)
	{
	}
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
	
	// font available on computer
	void AddFontLocal(int id, LPCWSTR faceName, FontData fontData);
	
	// custom file font (.tff only! Other file format is untested)
	void AddFontCustom(int id, LPCWSTR filePath, LPCWSTR faceName, FontData fontData);
	void Draw(int id, const FontDrawConfig& config);
	void Draw(int id, const Vector2& pos, LPCWSTR message, Color color);
	~FontManager();
}; 

