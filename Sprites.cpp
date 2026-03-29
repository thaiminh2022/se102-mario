#include "Sprites.h"

Sprites* Sprites::_instance = nullptr;

void Sprites::Add(int id, int left, int top, int right, int bottom, Texture* tex)
{
	Sprite* s = new Sprite(id, left, top, right, bottom, tex);
	sprites[id] = s;
}

Sprite* Sprites::Get(int id)
{
	return sprites[id];
}
