#pragma once
#include "GameObject.h"

class Mario : public GameObject
{
public:
	Mario(float startX, float startY);

	void Update(DWORD dt, vector<GameObject*>* coObjects) override;
	void Render() override;
};

