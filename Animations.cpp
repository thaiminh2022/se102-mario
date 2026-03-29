#include "Animations.h"


void Animations::Add(int id, Animation* ani)
{
	animations[id] = ani;
}

Animation* Animations::Get(int id)
{
	return animations[id];
}
