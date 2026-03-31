#pragma once
#include <unordered_map>

using std::unordered_map;

class InputManager
{
	static InputManager* _instance;
	bool keys[256];

	InputManager()
	{
		for (auto& key : keys)
		{
			key = false;
		}
	}

public:
	static InputManager* GetInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new InputManager;
		}

		return _instance;
	}
	void KeyDown(unsigned char key);
	void KeyUp(unsigned char key);
	bool IsKeyDown(unsigned char key);
	void ClearAll();
};

