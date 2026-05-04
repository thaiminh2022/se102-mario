#pragma once
#include <Windows.h>
#include <d3d10.h>
#include <D3DX10.h>
#include "Texture.h"
#include "Scene.h"
#include <unordered_map>
#include <xaudio2.h>

#include "Camera.h"
#include "Color.h"
#include "PlayableScene.h"

using std::unordered_map;

class Game
{
	static Game *_instance;

	HWND hWnd;

	int backBufferWidth = 0;
	int backBufferHeight = 0;

	ID3D10Device *device = nullptr;
	IDXGISwapChain *swapChain = nullptr;
	ID3D10RenderTargetView *renderTargetView = nullptr;
	ID3D10BlendState *blendStateAlpha = nullptr;
	ID3DX10Sprite *spriteObject = nullptr;
	ID3D10RasterizerState* rasterizerState = nullptr;

	int currentSceneID;
	int nextSceneID;
	unordered_map<int, Scene *> scenes;
	bool forceReload;

	vector<std::pair<Rect, D3DXCOLOR>> debugRects;
	
	Optional<SceneSwitchContext> sceneSwitchCtx;


	Optional<Color> bgColor;
	Camera* camera;
	Game() : hWnd(nullptr), currentSceneID(0), nextSceneID(0)
	{
		camera = new Camera;
		currentSceneID = -100;
		nextSceneID = -200;
		forceReload = false;
	}

public:
	static Game *GetInstance()
	{
		if (_instance == nullptr)
			_instance = new Game;

		return _instance;
	}

	void Init(HWND hWnd);
	void Draw(float x, float y, Texture* tex, Rect* rect = nullptr) const;
	void Draw(float x, float y, Texture *tex, int l, int t, int r, int b)
	{
		Rect rect;
		rect.left = l;
		rect.right = r;
		rect.top = t;
		rect.bottom = b;
		Draw(x, y, tex, &rect);
	}

	// Debug helper
	void DrawDebugRectRaw(Rect r, Color color);
	void DrawDebugRectWithCamera(Rect r, Color color);

	void FlushDebugRect();
	void ClearDebugRect() { debugRects.clear(); }

	Texture *LoadTexture(LPCWSTR texturePath) const;

	ID3D10Device *GetDirect3DDevice() const { return this->device; }
	IDXGISwapChain *GetSwapChain() const { return this->swapChain; }
	ID3D10RenderTargetView *GetRenderTargetView() const { return this->renderTargetView; }
	ID3DX10Sprite *GetSpriteHandler() const { return this->spriteObject; }
	ID3D10BlendState *GetAlphaBlending() const { return blendStateAlpha; }
	Optional<D3DXCOLOR> GetBackgroundColor() const;
	void SetBackgroundColor(const Optional<Color>& c);  


	int GetBackBufferWidth() const { return backBufferWidth; }
	int GetBackBufferHeight() const { return backBufferHeight; }
	Scene *GetCurrentScene() { return scenes[currentSceneID]; }

	// Scene related
	void SwitchScene();
	void IndicateSceneSwitch(int newID, const Optional<SceneSwitchContext>& ctx);
	void LoadSceneAndEnterFirst();
	void AddScene(int id, Scene* scene);
	bool HaveSceneWithID(int id);
	void ReloadCurrentScene();


	// Camera related	
	Camera* GetCamera() const { return camera; }

	~Game();
};
