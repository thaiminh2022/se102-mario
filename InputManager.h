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
		for (auto& key : previousKeys)
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

	bool IsKeyDownThisFrame(unsigned char key) const;
	bool IsKeyPressed(unsigned char key) const; //prev = false, current = true
	bool IsKeyDown(unsigned char key) const; // prev = true/false, current = true
	bool IsKeyReleased(unsigned char key) const; // prev = true, current = false
	void ClearAll();
};

