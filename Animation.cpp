#include "Animation.h"
#include "Sprites.h"
#include "Sprite.h"
#include "Debug.h"

void Animation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0)
		t = this->defaultTime;

	Sprite *sprite = Sprites::GetInstance()->Get(spriteId);
	if (sprite == NULL)
		DebugOut(L"[ERROR] Sprite ID %d not found!\n", spriteId);

	AnimationFrame *frame = new AnimationFrame(sprite, t);
	frames.push_back(frame);
}

void Animation::Render(float x, float y)
{
	ULONGLONG now = GetTickCount64();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size())
				currentFrame = 0;
		}
	}

	frames[currentFrame]->GetSprite()->Draw(x, y);
}

inline AnimationFrame::AnimationFrame(Sprite *sprite, int time)
{
	this->sprite = sprite;
	this->time = time;
}

inline DWORD AnimationFrame::GetTime() const { return time; }

inline Sprite *AnimationFrame::GetSprite() { return sprite; }
