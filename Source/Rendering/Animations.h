#pragma once
#include "Animation.h"
#include <memory>
#include <unordered_map>

using std::unordered_map;
using std::unique_ptr;

class Animations
{
	static Animations* _instance;
	unordered_map<int, unique_ptr<Animation>> animations;

public:
	static Animations* GetInstance() {
		if (_instance == nullptr)
			_instance = new Animations;

		return _instance;
	}

	void Add(int id, Animation* ani);
	void Add(int id, unique_ptr<Animation> ani);
	bool Contains(int id);
	Animation* Get(int id);

	void Clear() {
		animations.clear();
	}
	void Replace(int id, Animation* ani);
	void Replace(int id, unique_ptr<Animation> ani);
};

