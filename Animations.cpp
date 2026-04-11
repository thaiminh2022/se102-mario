#include "Animations.h"

Animations *Animations::_instance = nullptr;

void Animations::Add(int id, Animation *ani)
{
	if (animations.find(id) != animations.end())
		return;

	animations[id] = ani;
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
	return animations[id];
}
