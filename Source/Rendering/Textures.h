#pragma once

#include <D3DX10.h>
#include <memory>
#include <string>
#include <unordered_map>
#include "Texture.h"

using std::unordered_map;
using std::unique_ptr;

class Textures
{
	static Textures* _instance;

	unordered_map<int, unique_ptr<Texture>> textures;
	unordered_map<std::wstring, int> pathToTexture; // this is for caching

public:
	static Textures* GetInstance() {
		if (_instance == nullptr)
			_instance = new Textures;
		return _instance;
	}
	Textures();
	void Add(int id, LPCWSTR filePath);
	bool HaveTextureWithPath(const std::wstring& filePath, int& outID);
	Texture* Get(int i) { return textures[i].get(); }
};

