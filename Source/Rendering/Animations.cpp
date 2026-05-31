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
