#include "Game.h"
#include "Debug.h"
#include "Sprites.h"
#include "Animations.h"

Game* Game::_instance = nullptr;

void Game::Init(HWND hWnd)
{
	// retrieve client area width & height so that we can create backbuffer height & width accordingly
	RECT r;
	GetClientRect(hWnd, &r);

	backBufferWidth = r.right + 1;
	backBufferHeight = r.bottom + 1;

	// Create & clear the DXGI_SWAP_CHAIN_DESC structure
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Fill in the needed values
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = backBufferWidth;
	swapChainDesc.BufferDesc.Height = backBufferHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	// Create the D3D device and the swap chain
	HRESULT hr = D3D10CreateDeviceAndSwapChain(NULL,
		D3D10_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		D3D10_SDK_VERSION,
		&swapChainDesc,
		&swapChain,
		&device);

	if (hr != S_OK)
	{
		DebugOut(L"[ERROR] D3D10CreateDeviceAndSwapChain has failed %s %d", _W(__FILE__), __LINE__);
		return;
	}

	// Get the back buffer from the swapchain
	ID3D10Texture2D* pBackBuffer;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
	if (hr != S_OK)
	{
		DebugOut(L"[ERROR] pSwapChain->GetBuffer has failed %s %d", _W(__FILE__), __LINE__);
		return;
	}

	// create the render target view
	hr = device->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView);

	D3D10_RASTERIZER_DESC rsDesc = {};
	rsDesc.FillMode = D3D10_FILL_SOLID;
	rsDesc.CullMode = D3D10_CULL_NONE;
	rsDesc.FrontCounterClockwise = FALSE;
	rsDesc.DepthClipEnable = TRUE;
	hr = device->CreateRasterizerState(&rsDesc, &rasterizerState);
	if (FAILED(hr))
	{
		DebugOut(L"[Error] cannot init rasterizerState");
	}
	device->RSSetState(rasterizerState);

	pBackBuffer->Release();
	if (hr != S_OK)
	{
		DebugOut(L"[ERROR] CreateRenderTargetView has failed %s %d", _W(__FILE__), __LINE__);
		return;
	}

	// set the render target
	device->OMSetRenderTargets(1, &renderTargetView, NULL);

	// create and set the viewport
	D3D10_VIEWPORT viewPort;
	viewPort.Width = backBufferWidth;
	viewPort.Height = backBufferHeight;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	device->RSSetViewports(1, &viewPort);

	// create the sprite object to handle sprite drawing
	hr = D3DX10CreateSprite(device, 0, &spriteObject);

	if (hr != S_OK)
	{
		DebugOut(L"[ERROR] D3DX10CreateSprite has failed %s %d", _W(__FILE__), __LINE__);
		return;
	}

	D3DXMATRIX matProjection;

	// Create the projection matrix using the values in the viewport
	D3DXMatrixOrthoOffCenterLH(&matProjection,
		(float)viewPort.TopLeftX,
		(float)viewPort.Width,
		(float)viewPort.TopLeftY,
		(float)viewPort.Height,
		0.1f,
		10);
	hr = spriteObject->SetProjectionTransform(&matProjection);

	// Initialize the blend state for alpha drawing
	D3D10_BLEND_DESC StateDesc;
	ZeroMemory(&StateDesc, sizeof(D3D10_BLEND_DESC));
	StateDesc.AlphaToCoverageEnable = FALSE;
	StateDesc.BlendEnable[0] = TRUE;
	StateDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
	StateDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
	StateDesc.BlendOp = D3D10_BLEND_OP_ADD;
	StateDesc.SrcBlendAlpha = D3D10_BLEND_ZERO;
	StateDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
	StateDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	StateDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&StateDesc, &this->blendStateAlpha);


	DebugOut(L"[INFO] InitDirectX has been successful\n");


}

void Game::Draw(float x, float y, Texture* tex, Rect* rect)
{
	if (tex == nullptr)
		return;

	int spriteWidth = 0;
	int spriteHeight = 0;

	D3DX10_SPRITE sprite;

	// Set the sprite’s shader resource view
	sprite.pTexture = tex->GetShaderResourceView();

	if (rect == nullptr)
	{
		// top-left location in U,V coords
		sprite.TexCoord.x = 0;
		sprite.TexCoord.y = 0;

		// Determine the texture size in U,V coords
		sprite.TexSize.x = 1.0f;
		sprite.TexSize.y = 1.0f;

		spriteWidth = tex->GetWidth();
		spriteHeight = tex->GetHeight();
	}
	else
	{
		sprite.TexCoord.x = rect->left / (float)tex->GetWidth();
		sprite.TexCoord.y = rect->top / (float)tex->GetHeight();

		spriteWidth = (rect->right - rect->left + 1);
		spriteHeight = (rect->bottom - rect->top + 1);

		sprite.TexSize.x = spriteWidth / (float)tex->GetWidth();
		sprite.TexSize.y = spriteHeight / (float)tex->GetHeight();
	}

	// Set the texture index. Single textures will use 0
	sprite.TextureIndex = 0;

	// The color to apply to this sprite, full color applies white.
	sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//
	// Build the rendering matrix based on sprite location
	//

	// The translation matrix to be created
	D3DXMATRIX matTranslation;

	// Create the translation matrix

	D3DXMatrixTranslation(
		&matTranslation,
		x + spriteWidth * 0.5f,
		(backBufferHeight - y) - spriteHeight * 0.5f,
		0.1f
	);

	// Scale the sprite to its correct width and height because by default, DirectX draws it with width = height = 1.0f
	D3DXMATRIX matScaling;
	// flip X/Y by scaling with negative value on X/Y axis
	D3DXMatrixScaling(&matScaling, (FLOAT)spriteWidth, (FLOAT)spriteHeight, 1.0f); 

	// Setting the sprite’s position and size
	sprite.matWorld = (matScaling * matTranslation);

	spriteObject->DrawSpritesImmediate(&sprite, 1, 0, 0);
}

void Game::DrawDebugRect(Rect r, D3DXCOLOR color)
{
	const auto p = std::pair<Rect, D3DXCOLOR>(r, color);
	debugRects.push_back(p);
}

void Game::FlushDebugRect()
{
	if (debugRects.empty()) return;

	// Backup old blend state if needed
	float blendFactor[4] = { 0, 0, 0, 0 };
	UINT sampleMask = 0xffffffff;
	device->OMSetBlendState(blendStateAlpha, blendFactor, sampleMask);

	// We will draw using very small 1x1 white texture stretched up
	static Texture* whiteTex = nullptr;
	if (whiteTex == nullptr)
	{
		ID3D10Texture2D* tex = nullptr;

		D3D10_TEXTURE2D_DESC desc{};
		desc.Width = 1;
		desc.Height = 1;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D10_USAGE_IMMUTABLE;
		desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;

		unsigned int pixel = 0xffffffff; // white

		D3D10_SUBRESOURCE_DATA initData{};
		initData.pSysMem = &pixel;
		initData.SysMemPitch = sizeof(unsigned int);

		HRESULT hr = device->CreateTexture2D(&desc, &initData, &tex);
		if (FAILED(hr)) return;

		ID3D10ShaderResourceView* srv = nullptr;
		hr = device->CreateShaderResourceView(tex, nullptr, &srv);
		if (FAILED(hr))
		{
			tex->Release();
			return;
		}

		whiteTex = new Texture(tex, srv);
	}

	for (const auto& rectData : debugRects)
	{
		auto rect = rectData.first;
		auto color = rectData.second;

		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		if (width <= 0 || height <= 0) continue;


		D3DX10_SPRITE sprite{};
		sprite.pTexture = whiteTex->GetShaderResourceView();
		sprite.TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		sprite.TexSize = D3DXVECTOR2(1.0f, 1.0f);
		sprite.TextureIndex = 0;
		sprite.ColorModulate = color;

		D3DXMATRIX matTranslation, matScaling;
		D3DXMatrixTranslation(
			&matTranslation,
			rect.left + width * 0.5f,
			(backBufferHeight - rect.top) - height * 0.5f,
			0.1f
		);
		D3DXMatrixScaling(&matScaling, (FLOAT)width, (FLOAT)height, 1.0f);

		sprite.matWorld = matScaling * matTranslation;
		spriteObject->DrawSpritesImmediate(&sprite, 1, 0, 0);
	}
	debugRects.clear();
}


Texture* Game::LoadTexture(LPCWSTR texturePath) const
{
	ID3D10Resource* pD3D10Resource = NULL;
	ID3D10Texture2D* tex = NULL;

	// Loads the texture into a temporary ID3D10Resource object
	HRESULT hr = D3DX10CreateTextureFromFile(device,
		texturePath,
		nullptr, //&info,
		nullptr,
		&pD3D10Resource,
		nullptr);

	// Make sure the texture was loaded successfully
	if (FAILED(hr))
	{
		DebugOut(L"[ERROR] Failed to load texture file: %s with error: %d\n", texturePath, hr);
		return nullptr;
	}

	// Translates the ID3D10Resource object into a ID3D10Texture2D object
	auto _ = pD3D10Resource->QueryInterface(__uuidof(ID3D10Texture2D), (LPVOID*)&tex);
	pD3D10Resource->Release();

	if (!tex)
	{
		DebugOut(L"[ERROR] Failed to convert from ID3D10Resource to ID3D10Texture2D \n");
		return nullptr;
	}

	//
	// Create the Share Resource View for this texture
	//
	// Get the texture details
	D3D10_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);

	// Create a shader resource view of the texture
	D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;

	// Clear out the shader resource view description structure
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));

	// Set the texture format
	SRVDesc.Format = desc.Format;

	// Set the type of resource
	SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = desc.MipLevels;

	ID3D10ShaderResourceView* gSpriteTextureRV = NULL;

	device->CreateShaderResourceView(tex, &SRVDesc, &gSpriteTextureRV);

	DebugOut(L"[INFO] Texture loaded Ok from file: %s \n", texturePath);

	return new Texture(tex, gSpriteTextureRV);
}

void Game::SwitchScene()
{
	if (nextSceneID == currentSceneID)
		return;

	DebugOut(L"[INFO] Switching to scene %d\n", nextSceneID);
	if (scenes.find(currentSceneID) != scenes.end())
	{
		scenes[currentSceneID]->UnLoad();
	}
	Sprites::GetInstance()->Clear();
	Animations::GetInstance()->Clear();

	currentSceneID = nextSceneID;
	scenes[currentSceneID]->Load();
}

void Game::IndicateSceneSwitch(int newID)
{
	nextSceneID = newID;
}

void Game::LoadSceneAndEnterFirst()
{
	// level 1-1
	auto s1 = new PlayableScene(0);
	// level 1-2
	auto s2 = new PlayableScene(1);

	scenes[s1->GetID()] = s1;
	scenes[s2->GetID()] = s2;
	IndicateSceneSwitch(s1->GetID());
	SwitchScene();
}

Game::~Game()
{
	for (auto& v: scenes)
	{
		v.second->UnLoad();
	}


	delete camera;

	/// ===============D3D10 DESTROYS==================
	if (spriteObject) spriteObject->Release();
	if (blendStateAlpha) blendStateAlpha->Release();
	if (renderTargetView) renderTargetView->Release();
	if (swapChain) swapChain->Release();
	if (rasterizerState) rasterizerState->Release();


	// Others need to be destroyed before device
	if (device) device->Release();

	
	/// =================================================
	
	delete camera;
	camera = nullptr;
	for (auto& v: scenes)
	{
		delete v.second;
		v.second = nullptr;
	}
	scenes.clear();
}
