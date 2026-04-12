#include "Textures.h"
#include "Game.h"

Textures* Textures::_instance = nullptr;

Textures::Textures()
{
	textures = {};
}

void Textures::Add(int id, LPCWSTR filePath)
{
	if (pathToTexture.find(filePath) == pathToTexture.end())
	{
		pathToTexture[filePath] = id;
		textures[id] = Game::GetInstance()->LoadTexture(filePath);
	}

}

bool Textures::HaveTextureWithPath(std::wstring filePath, int& outID)
{
	if (pathToTexture.find(filePath) == pathToTexture.end())
	{
		return false;
	}

	outID = pathToTexture[filePath];
	return true;
}


