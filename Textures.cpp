#include "Textures.h"
#include "Game.h"

Textures* Textures::_instance = nullptr;

Textures::Textures()
{
	textures = {};
}

void Textures::Add(int id, LPCWSTR filePath)
{
	textures[id] = Game::GetInstance()->LoadTexture(filePath);
}

Texture* Textures::Get(unsigned int i)
{
	return nullptr;
}

Textures::~Textures()
{
}
