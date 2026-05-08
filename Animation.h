#pragma once

#include <Windows.h>
#include <vector>
#include "Sprite.h"

using std::vector;
class AnimationFrame {
	Sprite* sprite;
	DWORD time;
public:
	AnimationFrame(Sprite* sprite, int timeMs);
	DWORD GetTime() const;
	Sprite* GetSprite() const;
};

class Animation
{
	ULONGLONG lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<AnimationFrame*> frames;

public:
	Animation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, DWORD timeMs = 0);
	void Render(float x, float y, bool flipX, bool flipY, int startFrame = -1);
};

