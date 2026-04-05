#include "InputManager.h"

#include "Debug.h"

InputManager* InputManager::_instance = nullptr;

void InputManager::KeyDown(const unsigned char key)
{
	keys[key] = true;
}

void InputManager::KeyUp(const unsigned char key)
{
	keys[key] = false;
}

bool InputManager::IsKeyDown(const unsigned char key) const
{
	//DebugOut(L"key: %d: %d\n", key, keys[key]);
	return keys[key];
}

void InputManager::ClearAll()
{
	for (bool& key : keys)
	{
		key = false;
	}
}
