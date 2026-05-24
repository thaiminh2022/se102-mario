#include "Animation.h"
#include "Sprites.h"
#include "Sprite.h"
#include "Debug.h"

void Animation::Add(int spriteId, DWORD timeMs)
{
	int t = timeMs;
	if (timeMs == 0)
		t = this->defaultTime;

	Sprite *sprite = Sprites::GetInstance()->Get(spriteId);
	if (sprite == nullptr)
		DebugOut(L"[ERROR] Sprite ID %d not found!\n", spriteId);

	frames.emplace_back(sprite, t);
}

void Animation::Render(float x, float y, bool flipX, bool flipY, int startFrame)
{
	ULONGLONG now = GetTickCount64();
	if (startFrame != -1 && startFrame < frames.size())
	{
		currentFrame = startFrame;
		lastFrameTime = now;
	}
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame].GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size())
				currentFrame = 0;
		}
	}

	frames[currentFrame].GetSprite()->Draw(x, y, flipX, flipY);
}

inline AnimationFrame::AnimationFrame(Sprite *sprite, int timeMs)
{
	this->sprite = sprite;
	this->time = timeMs;
}

inline DWORD AnimationFrame::GetTime() const { return time; }

inline Sprite *AnimationFrame::GetSprite() const { return sprite; }
