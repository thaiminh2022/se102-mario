#pragma once
#include <Windows.h>
#include <vector>

using std::vector;

class GameObject
{
protected:
	float x, y;
	float vx, vy;
	bool isDeleted;

public:
	GameObject() { x = 0; y = 0; isDeleted = false; vx = 0; vy = 0; }
	GameObject(float x, float y) { this->x = x; this->y = y; isDeleted = false; vx = 0; vy = 0; }

	virtual void GetPosition(float& x, float& y) const;

	virtual void Update(DWORD dt, vector<GameObject*>* coObjects = nullptr) = 0;
	virtual void Render() = 0;
	virtual bool HaveCollision() { return true; }
	static bool IsDeleted(const GameObject* o) { return o->isDeleted; }
	virtual ~GameObject() = default;
};

