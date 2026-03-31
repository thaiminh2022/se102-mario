#include "PlayableScene.h"

#include "Game.h"

void PlayableScene::Update(DWORD dt)
{
	// coObjects is all object, not including obj itself
	vector<GameObject*> coObjects;
	for (const auto& obj : objects)
	{
		for (auto other : objects)
		{
			if (other != obj)
				coObjects.push_back(other);
		}

		obj->Update(dt, &coObjects);
		coObjects.clear();
	}

	CleanupDeletedObjects();
}

void PlayableScene::Load()
{
	auto tilemap = LevelLoader::GetInstance()->GetTilemapForLevel(0);

	player = new Mario(tilemap->playerStartX, tilemap->playerStartY);
	objects.push_back(player);
}

void PlayableScene::UnLoad()
{
	for (auto& ob : objects)
	{
		delete ob;
		ob = nullptr;
	}
	objects.clear();
}

void PlayableScene::Render()
{

	LevelLoader::GetInstance()->GetTilemapForLevel(0)->Render();

	for (const auto &obj : objects)
	{
		obj->Render();
	}
}

void PlayableScene::CleanupDeletedObjects()
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		GameObject* o = *it;
		if (GameObject::IsDeleted(o))
		{
			delete o;
			*it = nullptr;
		}
	}

	objects.erase(
		std::remove_if(objects.begin(), objects.end(),
		[](const GameObject* o)
		{
				return o == nullptr;
		}),
		objects.end());
}
