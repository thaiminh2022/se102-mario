#pragma once
#include "Animation.h"
#include <unordered_map>

using std::unordered_map;

class Animations
{
	static Animations* _instance;
	unordered_map<int, Animation*> animations;

public:
	static Animations* GetInstance() {
		if (_instance == nullptr)
			_instance = new Animations;

		return _instance;
	}

	void Add(int id, Animation* ani);
	Animation* Get(int id);
};

