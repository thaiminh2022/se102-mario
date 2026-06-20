#include "Animations.h"
#include <utility>

Animations *Animations::_instance = nullptr;

void Animations::Add(int id, Animation *ani)
{
	Add(id, unique_ptr<Animation>(ani));
}

void Animations::Add(int id, unique_ptr<Animation> ani)
{
	if (animations.find(id) != animations.end())
		return;

	animations[id] = std::move(ani);
}

bool Animations::Contains(int id)
{
	if (animations.find(id) != animations.end())
	{
		return true;
	}
	return false;
}

Animation *Animations::Get(int id)
{
	return animations[id].get();
}

// Overload taking a raw pointer
void Animations::Replace(int id, Animation* ani)
{
	if (!ani) return;

	auto it = animations.find(id);
	if (it != animations.end())
	{
		Animation* oldAni = it->second.get();

		if (oldAni != nullptr)
		{
			ani->SetCurrentFrame(oldAni->GetCurrentFrame());
			ani->SetLastFrameTime(oldAni->GetLastFrameTime());
		}
		it->second.reset(ani);
	}
	else
	{
		Add(id, ani);
	}
}

// Overload taking a unique_ptr (Move semantics)
void Animations::Replace(int id, std::unique_ptr<Animation> newAni)
{
	if (!newAni) return;

	auto it = animations.find(id);
	if (it != animations.end())
	{
		Animation* oldAni = it->second.get();

		if (oldAni != nullptr)
		{
			newAni->SetCurrentFrame(oldAni->GetCurrentFrame());

			newAni->SetLastFrameTime(oldAni->GetLastFrameTime());
		}
		animations[id] = std::move(newAni);
	}
	else
	{
		animations[id] = std::move(newAni);
	}
}
