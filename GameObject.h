#pragma once
#include <Windows.h>
#include <vector>

using std::vector;

class GameObject
{
protected:
	float x;
	float y;
	bool isDeleted;

public:
	GameObject() { x = 0; y = 0; isDeleted = false; }
	GameObject(float x, float y) { this->x = x; this->y = y; isDeleted = false; }

	virtual void Update(DWORD dt, vector<GameObject*>* coObjects = nullptr) = 0;
	virtual void Render() = 0;
	static bool IsDeleted(GameObject* o) { return o->isDeleted; }
	virtual ~GameObject() {}
};

