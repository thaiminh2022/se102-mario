#pragma once

#include <D3DX10.h>
#include <unordered_map>
#include "Texture.h"

using std::unordered_map;

class Textures
{
	static Textures* _instance;

	unordered_map<int, Texture*> textures;

public:
	static Textures* GetInstance() {
		if (_instance == nullptr)
			_instance = new Textures;
		return _instance;
	}
	Textures();
	void Add(int id, LPCWSTR filePath);
	Texture* Get(unsigned int i);

	~Textures();
};

