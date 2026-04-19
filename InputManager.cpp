#include "InputManager.h"

InputManager* InputManager::_instance = nullptr;

void InputManager::KeyDown(const unsigned char key)
{
	currentKeys[key] = true;
}

void InputManager::KeyUp(const unsigned char key)
{
	currentKeys[key] = false;
}

bool InputManager::IsKeyDownThisFrame(unsigned char key) const
{
	return currentKeys[key] &&!previousKeys[key];
}

void InputManager::Update()
{
	std::memcpy(previousKeys, currentKeys, sizeof(currentKeys));
}
bool InputManager::IsKeyDown(const unsigned char key) const
{
	//DebugOut(L"key: %d: %d\n", key, keys[key]);
	return currentKeys[key];
}

bool InputManager::IsKeyPressed(const unsigned char key) const
{
	return currentKeys[key] && !previousKeys[key];
}

bool InputManager::IsKeyReleased(const unsigned char key) const
{
	return !currentKeys[key] && previousKeys[key];
}

void InputManager::ClearAll()
{
	std::memset(currentKeys, 0, sizeof(currentKeys));
	std::memset(previousKeys, 0, sizeof(previousKeys));
}
