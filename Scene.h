#pragma once
#include <Windows.h>
#include <vector>

using std::vector;

class Scene
{
protected:
	int id;

public:
	int GetID() { return id; }

	Scene() { id = -1; }
	Scene(int id) { this->id = id; }
	virtual void Update(DWORD dt) {}
	virtual void Render() {}
	virtual void Load() {}
	virtual void UnLoad() {}

	virtual  ~Scene(){}
};
