#include "Sprite.h"
#include "Sprites.h"
#include "Texture.h"
#include <memory>

Sprites* Sprites::_instance = nullptr;

void Sprites::Add(int id, int left, int top, int right, int bottom, Texture* tex)
{
	if (sprites.find(id) != sprites.end())
		return;

	sprites[id] = std::make_unique<Sprite>(id, left, top, right, bottom, tex);
}

Sprite* Sprites::Get(int id)
{
	return sprites[id].get();
}
