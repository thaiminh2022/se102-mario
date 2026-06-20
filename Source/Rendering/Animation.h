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
	vector<AnimationFrame> frames;

public:
	Animation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	Animation(int spriteIds[], int n, int defaultTime)
	{
		this->defaultTime = defaultTime;
		lastFrameTime = -1; 
		currentFrame = -1;

		for (int i = 0; i < n; i++)
		{
			Add(spriteIds[i]);
		}
	}
	void Add(int spriteId, DWORD timeMs = 0);
	void Render(float x, float y, bool flipX, bool flipY, int startFrame = -1);

	int GetCurrentFrame() const { return currentFrame; }
	void SetCurrentFrame(int frame) { currentFrame = frame; }

	ULONGLONG GetLastFrameTime() const { return lastFrameTime; }
	void SetLastFrameTime(ULONGLONG time) { lastFrameTime = time; }
	Animation* Clone();
};

