#pragma once

#include <d3d10.h>
#include <D3DX10.h>

class Texture
{
protected:
	ID3D10Texture2D* tex;
	ID3D10ShaderResourceView* rsView;
	int width, height;

public:
	Texture()
	{
		tex = nullptr;
		rsView = nullptr;
		width = -1;
		height = -1;
	}

	Texture(ID3D10Texture2D* tex, ID3D10ShaderResourceView* rsview)
	{
		this->tex = tex;
		this->rsView = rsview;

		D3D10_TEXTURE2D_DESC desc;
		this->tex->GetDesc(&desc);
		this->width = desc.Width;
		this->height = desc.Height;
	}

	int GetWidth() const { return width; };
	int GetHeight() const { return height; };

	ID3D10ShaderResourceView* GetShaderResourceView() { return this->rsView; }

	~Texture()
	{
		if (rsView != nullptr) this->rsView->Release();
		if (tex != nullptr) this->tex->Release();
	}
};

