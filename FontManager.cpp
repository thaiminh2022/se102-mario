#include "FontManager.h"

#include "Debug.h"
#include "Game.h"

FontManager* FontManager::_instance = nullptr;

void FontManager::AddFontLocal(int id, LPCWSTR faceName, FontData fontData)
{
	auto dv = Game::GetInstance()->GetDirect3DDevice();
	ID3DX10Font* font = nullptr;
	auto hr = D3DX10CreateFont(dv, 
		fontData.height, 
		fontData.width, 
		fontData.weight, 
		1, 
		fontData.italic, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		ANTIALIASED_QUALITY, 
		FF_DONTCARE, 
		faceName, 
		&font
	);

	if (FAILED(hr))
	{
		DebugOut(L"[Error] Cannot load font");
		return;
	}

	fonts[id] = font;
}

void FontManager::AddFontCustom(int id, LPCWSTR filePath, LPCWSTR faceName, FontData fontData)
{
	int status = AddFontResourceEx(filePath, FR_PRIVATE, nullptr);
	if (status == 0)
	{
		DebugOut(L"[Error] Cannot load custom font");
		return;
	}
	ID3DX10Font* font = nullptr;
	HRESULT hr = D3DX10CreateFont(
		Game::GetInstance()->GetDirect3DDevice(),
		fontData.height,
		fontData.width,
		fontData.weight,
		1,
		fontData.italic,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		NONANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		faceName,  
		&font
	);

	if (FAILED(hr))
	{
		DebugOut(L"[Error] Cannot load font");
		return;
	}

	fonts[id] = font;
}

void FontManager::Draw(const int id, const FontDrawConfig& config)
{
	auto r = config.r.GetRECT();
	const auto& font = fonts[id];

	font->DrawTextW(
		Game::GetInstance()->GetSpriteHandler(),
		config.message,
		-1,
		&r,
		config.format,
		config.color
	);
}

void FontManager::Draw(const int id, const Vector2& pos, const LPCWSTR message, const D3DXCOLOR color)
{

	const auto& font = fonts[id];
	auto r = Rect::FromXYWH(pos.x, pos.y, 2000, 2000).GetRECT();

	font->DrawTextW(
		Game::GetInstance()->GetSpriteHandler(),
		message,
		-1,
		&r,
		TextFormat::Left | TextFormat::NoClip,
		color
	);
}

FontManager::~FontManager()
{
	for (auto& it : fonts)
	{
		it.second->Release();
		it.second = nullptr;
	}
	fonts.clear();
}
