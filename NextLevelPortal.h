#pragma once
#include "GameObject.h"

class NextLevelPortal : public GameObject
{
	Rect zone;
	int levelToLoad;

public:
	NextLevelPortal(Rect zone, int levelToLoad);
	void RequestNextLevel() const;


	void Render() override {}
	bool IsCollidable() override { return true; }
	bool IsBlocking() override { return false; }
	Rect GetBoundingBox() override { return zone; }
};

