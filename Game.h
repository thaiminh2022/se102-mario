#pragma once
#include <Windows.h>
#include <d3d10.h>
#include <D3DX10.h>
#include "Texture.h"

class Game
{
	static Game* _instance;

	HWND hWnd;									

	int backBufferWidth = 0;					
	int backBufferHeight = 0;

	ID3D10Device* device = NULL;
	IDXGISwapChain* swapChain = NULL;
	ID3D10RenderTargetView* renderTargetView = NULL;
	ID3D10BlendState* blendStateAlpha = NULL;		
	ID3DX10Sprite* spriteObject = NULL;			

public:
	static Game* GetInstance() {
		if (_instance == nullptr)
			_instance = new Game;

		return _instance;
	}

	void Init(HWND hWnd);
	void Draw(float x, float y, Texture* tex, RECT* rect = nullptr);
	void Draw(float x, float y, Texture* tex, int l, int t, int r, int b) {
		RECT rect;
		rect.left = l;
		rect.right = r;
		rect.top = t;
		rect.bottom = b;
		Draw(x, y, tex, &rect);
	}

	Texture* LoadTexture(LPCWSTR texturePath);

	ID3D10Device* GetDirect3DDevice() { return this->device; }
	IDXGISwapChain* GetSwapChain() { return this->swapChain; }
	ID3D10RenderTargetView* GetRenderTargetView() { return this->renderTargetView; }
	ID3DX10Sprite* GetSpriteHandler() { return this->spriteObject; }
	ID3D10BlendState* GetAlphaBlending() { return blendStateAlpha; };

	int GetBackBufferWidth() { return backBufferWidth; }
	int GetBackBufferHeight() { return backBufferHeight; }

	~Game();
};

