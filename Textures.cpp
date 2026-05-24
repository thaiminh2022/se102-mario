#include "Textures.h"
#include "Game.h"

Textures* Textures::_instance = nullptr;

Textures::Textures()
{
}

void Textures::Add(const int id, const LPCWSTR filePath)
{
	if (pathToTexture.find(filePath) == pathToTexture.end())
	{
		pathToTexture[filePath] = id;
		textures[id] = unique_ptr<Texture>(Game::GetInstance()->LoadTexture(filePath));
	}

}

bool Textures::HaveTextureWithPath(const std::wstring& filePath, int& outID)
{
	if (pathToTexture.find(filePath) == pathToTexture.end())
	{
		return false;
	}

	outID = pathToTexture[filePath];
	return true;
}


