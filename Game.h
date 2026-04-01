#pragma once
#include <Windows.h>
#include <d3d10.h>
#include <D3DX10.h>
#include "Texture.h"
#include "Scene.h"
#include <unordered_map>

#include "Camera.h"
#include "PlayableScene.h"

using std::unordered_map;

class Game
{
	static Game *_instance;

	HWND hWnd;

	int backBufferWidth = 0;
	int backBufferHeight = 0;

	ID3D10Device *device = NULL;
	IDXGISwapChain *swapChain = NULL;
	ID3D10RenderTargetView *renderTargetView = NULL;
	ID3D10BlendState *blendStateAlpha = NULL;
	ID3DX10Sprite *spriteObject = NULL;

	int currentSceneID;
	int nextSceneID;
	unordered_map<int, Scene *> scenes;

	Camera* camera;
	Game() : hWnd(nullptr), currentSceneID(0), nextSceneID(0)
	{
		camera = new Camera;
	}

public:
	static Game *GetInstance()
	{
		if (_instance == nullptr)
			_instance = new Game;

		return _instance;
	}

	void Init(HWND hWnd);
	void Draw(float x, float y, Texture *tex, RECT *rect = nullptr);
	void Draw(float x, float y, Texture *tex, int l, int t, int r, int b)
	{
		RECT rect;
		rect.left = l;
		rect.right = r;
		rect.top = t;
		rect.bottom = b;
		Draw(x, y, tex, &rect);
	}

	Texture *LoadTexture(LPCWSTR texturePath) const;

	ID3D10Device *GetDirect3DDevice() const { return this->device; }
	IDXGISwapChain *GetSwapChain() const { return this->swapChain; }
	ID3D10RenderTargetView *GetRenderTargetView() const { return this->renderTargetView; }
	ID3DX10Sprite *GetSpriteHandler() const { return this->spriteObject; }
	ID3D10BlendState *GetAlphaBlending() const { return blendStateAlpha; };

	int GetBackBufferWidth() const { return backBufferWidth; }
	int GetBackBufferHeight() const { return backBufferHeight; }
	Scene *GetCurrentScene() { return scenes[currentSceneID]; }

	// Scene related
	void SwitchScene();
	void IndicateSceneSwitch(int newID);
	void EnterStartingScene();

	// Camera related	
	Camera* GetCamera() const { return camera; }

	~Game();
};
