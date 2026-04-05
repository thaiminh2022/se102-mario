#include "Animations.h"

Animations *Animations::_instance = nullptr;

void Animations::Add(int id, Animation *ani)
{
	if (animations.find(id) != animations.end())
		return;

	animations[id] = ani;
}

Animation *Animations::Get(int id)
{
	return animations[id];
}
