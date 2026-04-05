#pragma once
#include "Sprite.h"
#include "Texture.h"
#include <unordered_map>

using std::unordered_map;

class Sprites
{
	static Sprites* _instance;
	unordered_map<int, Sprite*> sprites;

public:
	static Sprites* GetInstance() {
		if (_instance == nullptr)
			_instance = new Sprites;

		return _instance;
	}

	void Add(int id, int left, int top, int right, int bottom, Texture* tex);
	Sprite* Get(int id);

	void Clear() {
		for (auto& v: sprites)
		{
			delete v.second;
			v.second = nullptr;
		}
		sprites.clear();
	}

};

