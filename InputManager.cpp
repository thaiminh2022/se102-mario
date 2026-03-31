#include "InputManager.h"

#include "Debug.h"

InputManager* InputManager::_instance = nullptr;

void InputManager::KeyDown(unsigned char key)
{
	keys[key] = true;
}

void InputManager::KeyUp(unsigned char key)
{
	keys[key] = false;
}

bool InputManager::IsKeyDown(unsigned char key)
{
	DebugOut(L"key: %d: %d\n", key, keys[key]);
	return keys[key];
}

void InputManager::ClearAll()
{
	for (bool& key : keys)
	{
		key = false;
	}
}
