#pragma once
#include <Windows.h>
#include <d3d10.h>
#include <D3DX10.h>
#include <memory>
#include <wrl/client.h>
#include "Texture.h"
#include "Scene.h"
#include <unordered_map>
#include <xaudio2.h>

#include "Camera.h"
#include "Color.h"
#include "PlayableScene.h"

using std::unordered_map;
using std::unique_ptr;
using Microsoft::WRL::ComPtr;

class Game
{
	static Game *_instance;

	HWND hWnd;

	int backBufferWidth = 0;
	int backBufferHeight = 0;

	ComPtr<ID3D10Device> device;
	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<ID3D10RenderTargetView> renderTargetView;
	ComPtr<ID3D10BlendState> blendStateAlpha;
	ComPtr<ID3DX10Sprite> spriteObject;
	ComPtr<ID3D10RasterizerState> rasterizerState;

	int currentSceneID;
	int nextSceneID;
	unordered_map<int, unique_ptr<Scene>> scenes;
	bool forceReload;

	vector<std::pair<Rect, D3DXCOLOR>> debugRects;
	
	Optional<SceneSwitchContext> sceneSwitchCtx;


	Optional<Color> bgColor;
	unique_ptr<Camera> camera;
	Game() : hWnd(nullptr), currentSceneID(0), nextSceneID(0)
	{
		camera = std::make_unique<Camera>();
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

	ID3D10Device *GetDirect3DDevice() const { return device.Get(); }
	IDXGISwapChain *GetSwapChain() const { return swapChain.Get(); }
	ID3D10RenderTargetView *GetRenderTargetView() const { return renderTargetView.Get(); }
	ID3DX10Sprite *GetSpriteHandler() const { return spriteObject.Get(); }
	ID3D10BlendState *GetAlphaBlending() const { return blendStateAlpha.Get(); }
	Optional<D3DXCOLOR> GetBackgroundColor() const;
	void SetBackgroundColor(const Optional<Color>& c);  


	int GetBackBufferWidth() const { return backBufferWidth; }
	int GetBackBufferHeight() const { return backBufferHeight; }
	Scene *GetCurrentScene() { return scenes[currentSceneID].get(); }

	// Scene related
	void SwitchScene();
	void IndicateSceneSwitch(int newID, const Optional<SceneSwitchContext>& ctx);
	void LoadSceneAndEnterFirst();
	void AddScene(int id, Scene* scene);
	void AddScene(int id, unique_ptr<Scene> scene);
	bool HaveSceneWithID(int id);
	void ReloadCurrentScene();


	// Camera related	
	Camera* GetCamera() const { return camera.get(); }

	~Game();
};
