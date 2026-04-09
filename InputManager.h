#pragma once
#include <unordered_map>

using std::unordered_map;

class InputManager
{
	static InputManager* _instance;
	bool currentKeys[256];
	bool previousKeys[256];

	InputManager()
	{
		for (auto& key : currentKeys)
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
	void Update();

	void KeyDown(unsigned char key);
	void KeyUp(unsigned char key);

	bool IsKeyPressed(const unsigned char key) const; //prev = false, current = true
	bool IsKeyDown(unsigned char key) const; // prev = true/false, current = true
	bool IsKeyReleased(const unsigned char key) const; // prev = true, current = false
	void ClearAll();
};

