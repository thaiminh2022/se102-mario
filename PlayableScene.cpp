#include "Game.h"
#include "GameObject.h"
#include "LevelLoader.h"
#include "Mario.h"
#include "PlayableScene.h"
#include "Scene.h"
#include <algorithm>
#include <vector>

#include "AudioManager.h"
#include "AssetIDs.h"
#include "Coin.h"
#include "FireballTrap.h"
#include "FlagPole.h"
#include "FontManager.h"
#include "Goomba.h"
#include "NextLevelPortal.h"
#include "PointPopup.h"
#include "QuestionBlock.h"


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
				if (GameObject::IsDeleted(other)) continue;
				coObjects.push_back(other);
			}
		}

		obj->Update(dt, coObjects, ctx);
	}
	Game::GetInstance()->GetCamera()->Update();
	CleanupDeletedObjects();

	while (!addPendingGos.empty())
	{
		auto& g = addPendingGos.front();
		objects.push_back(g);
		addPendingGos.pop();
	}
}

void PlayableScene::Load()
{
	if (ctx == nullptr)
	{
		ctx = new SceneContext;
	}
	ctx->tilemap = LevelLoader::GetInstance()->GetTilemapForLevel(level);
	ctx->addObject =[this](GameObject *go)
	{
		AddObject(go);
	};
	ctx->addPointPopup = [this](const Vector2& pos, int value)
	{
		AddObject(new PointPopup(pos, value));
	};

	auto config = ctx->tilemap->GetConfig();

	// camera
	auto c = Game::GetInstance()->GetCamera();
	c->SetWorldSize(config->worldWidth, config->worldHeight);

	// player
	auto playerStart = config->entityData.playerStarts;
	ctx->mario = new Mario(playerStart.x, playerStart.y);

	c->SetTarget(ctx->mario);
	objects.push_back(ctx->mario);

	// goomba
	for (const auto& gPos : config->entityData.goombaStarts)
	{
		const auto gb = new Goomba(gPos.x, gPos.y);
		objects.push_back(gb);
	}

	// question

	for (const auto& qbData : config->entityData.questionBlocks)
	{
		const auto qb = new QuestionBlock(qbData.position, qbData.dropType);
		objects.push_back(qb);
	}

	//bricks
	for (const auto& qbData : config->entityData.brickBlocks)
	{
		const auto qb = new QuestionBlock(qbData.position, qbData.dropType, true, qbData.isHidden);
		objects.push_back(qb);
	}


	// coins
	for (const auto& cPos : config->entityData.coins)
	{
		const auto coin = new Coin(cPos);
		objects.push_back(coin);
	}

	// next level portal
	for (const auto& pPos : config->entityData.nextLevelsData)
	{
		const auto portal = new NextLevelPortal(pPos.zone, pPos.levelToLoad, pPos.delaySeconds);
		objects.push_back(portal);
	}

	// fire trap
	for (const auto& pPos : config->entityData.fireballTraps)
	{
		const auto trap = new FireballTrap(pPos);
		objects.push_back(trap);
	}

	// flagpole
	if (config->entityData.flagPole.hasValue)
	{
		const auto flag = config->entityData.flagPole.value;
		objects.push_back(new FlagPole(flag.zone, flag.moveToPosition));
	}

	// background music
	if (config->entityData.backgroundMusicID.hasValue)
	{
		AudioManager::GetInstance()->PlayMusic(config->entityData.backgroundMusicID.value);
	}

	// background color
	Game::GetInstance()->SetBackgroundColor(config->backgroundColor);
}

void PlayableScene::UnLoad()
{
	AudioManager::GetInstance()->StopAll();

	for (auto& ob : objects)
	{
		delete ob;
		ob = nullptr;
	}
	objects.clear();
}

void PlayableScene::Render()
{
	LevelLoader::GetInstance()->GetTilemapForLevel(level)->Render();

	std::sort(objects.begin(), objects.end(), GameObject::SortRenderIndex);
	for (const auto& obj : objects)
	{
		obj->Render();
	}

	wchar_t scoreText[32];
	swprintf_s(scoreText, L"SCORE %06d", Mario::GetScore());
	FontManager::GetInstance()->Draw(STATS_FONT, Vector2(8, 8), scoreText, Colors::WHITE);
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

void PlayableScene::AddObject(GameObject* go)
{
	addPendingGos.push(go);
}
