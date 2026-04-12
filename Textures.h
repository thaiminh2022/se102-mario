#pragma once

#include <D3DX10.h>
#include <string>
#include <unordered_map>
#include "Texture.h"

using std::unordered_map;

class Textures
{
	static Textures* _instance;

	unordered_map<int, Texture*> textures;
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
	Texture* Get(int i) { return textures[i]; }
};

