#include "PlayableScene.h"
#include "Game.h"

void PlayableScene::Update(DWORD dt)
{
	// coObjects is all object, not including obj itself
	vector<GameObject*> coObjects;
	for (const auto& obj : objects)
	{
		// make co-objects
		coObjects.clear();
		if (obj->IsCollidable())
		{

			for (auto other : objects)
			{
				if (!other->IsCollidable()) continue;
				if (other == obj) continue;

				coObjects.push_back(other);
			}
		}

		obj->Update(dt, coObjects, ctx);
	}
	Game::GetInstance()->GetCamera()->Update();
	CleanupDeletedObjects();
}

void PlayableScene::Load()
{
	if (ctx== nullptr)
	{
		ctx = new SceneContext;
	}

	ctx->tilemap = LevelLoader::GetInstance()->GetTilemapForLevel(0);
	auto c = Game::GetInstance()->GetCamera();
	
	c->SetWorldSize(ctx->tilemap->GetWidth(), ctx->tilemap->GetHeight());
	
	int startX, startY;
	ctx->tilemap->GetPlayerStartPosition(startX, startY);
	player = new Mario(startX, startY);
	
	c->SetTarget(player);
	
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
	delete player;
	player = nullptr;
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
