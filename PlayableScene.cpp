#include "Game.h"
#include "GameObject.h"
#include "LevelLoader.h"
#include "Mario.h"
#include "PlayableScene.h"
#include "Scene.h"
#include <algorithm>
#include <vector>

#include "Goomba.h"
#include "NextLevelPortal.h"


void PlayableScene::Update(float dt)
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

	ctx->tilemap = LevelLoader::GetInstance()->GetTilemapForLevel(id);
	auto config = ctx->tilemap->GetConfig();

	auto c = Game::GetInstance()->GetCamera();
	
	c->SetWorldSize(config->worldWidth, config->worldHeight);
	auto playerStart = config->entityData.playerStarts;
	player = new Mario(playerStart.x, playerStart.y);
	c->SetTarget(player);
	objects.push_back(player);

	// goomba
	for (const auto& gPos : config->entityData.goombaStarts)
	{
		const auto gb = new Goomba(gPos.x, gPos.y);
		objects.push_back(gb);
	}

	// next level portal
	for (const auto& pPos : config->entityData.nextLevelsData)
	{
		const auto portal = new NextLevelPortal(pPos.zone, pPos.levelToLoad);
		objects.push_back(portal);
	}
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
	LevelLoader::GetInstance()->GetTilemapForLevel(id)->Render();

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
