#include "Game.h"
#include "GameObject.h"
#include "LevelLoader.h"
#include "Mario.h"
#include "PlayableScene.h"
#include "Scene.h"
#include <algorithm>
#include <vector>

#include "AudioManager.h"
#include "Bloopers.h"
#include "Coin.h"
#include "FireballTrap.h"
#include "FlagPole.h"
#include "Goomba.h"
#include "Koopa.h"
#include "NextLevelPortal.h"
#include "Pipe.h"
#include "Bridge.h"
#include "QuestionBlock.h"
#include "HUD.h"
#include <queue>
#include "Bowser.h"

#include "BgMusicTrigger.h"
#include "CheepCheeps.h"
#include "ClearScreenColorTrigger.h"
#include "InWaterTrigger.h"
#include "FireShooter.h"


using std::priority_queue;
using std::pair;

typedef pair<int, std::function<void()>> render_item;


struct RenderCompare
{
	bool operator()(const render_item& a, const render_item& b) const
	{
		return a.first > b.first;
	}
};

typedef priority_queue<render_item, vector<render_item>, RenderCompare> render_queue;

void PlayableScene::Update(float dt)
{
	// coObjects is all object, not including obj itself
	vector<GameObject*> coObjects;
	for (const auto& obj : objects)
	{
		auto box = obj->GetBoundingBox();
		auto inView = Game::GetInstance()->GetCamera()->IsInView(box);
		if (inView)
		{
			//Game::GetInstance()->DrawDebugRectWithCamera(box, Colors::GREEN.WithAlpha(0.3f));
			obj->SetActive(true);
		}
		else
		{
			obj->SetActive(false);
		}

		if (!obj->IsActive())
		{
			continue;
		}

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

		obj->Update(dt, coObjects, sceneContext);
	}
	Game::GetInstance()->GetCamera()->Update();
	CleanupDeletedObjects();

	while (!addPendingGos.empty())
	{
		auto& g = addPendingGos.front();
		objects.push_back(g);
		addPendingGos.pop();
	}
	HUD::GetInstance()->Update(dt);
	levelTimer->ProcessTimer(dt);
	HUD::GetInstance()->GetElement(3)->SetText(L"TIME\n" + std::to_wstring(static_cast<int>(levelTimer->GetTimeLeft())));
	if (levelTimer->IsFinished())
	{
		// Time's up, kill Mario
		auto mario = sceneContext->mario;
		if (mario != nullptr) {
			mario->Die();
		}
	}
}

void PlayableScene::Load(const Optional<SceneSwitchContext>& ctx)
{
	if (sceneContext == nullptr)
	{
		sceneContext = new SceneContext;
	}
	sceneContext->tilemap = LevelLoader::GetInstance()->GetTilemapForLevel(level);
	sceneContext->addObject = [this](GameObject* go)
		{
			AddObject(go);
		};

	auto config = sceneContext->tilemap->GetConfig();

	// camera
	auto c = Game::GetInstance()->GetCamera();
	c->SetWorldSize(config->worldWidth, config->worldHeight);

	// player
	auto playerStart = config->entityData.playerStarts;
	sceneContext->mario = new Mario(playerStart.x, playerStart.y);
	objects.push_back(sceneContext->mario);
	if (ctx.hasValue)
	{
		sceneContext->mario->SetPowerLevel(ctx.value.marioPower);
		if (ctx.value.marioCtx.hasValue)
		{
			sceneContext->mario->SetExitPipe(ctx.value.marioCtx.value);
		}
	}

	c->SetTarget(sceneContext->mario);

	// goomba
	for (const auto& gPos : config->entityData.goombaStarts)
	{
		const auto gb = new Goomba(gPos.x, gPos.y);
		objects.push_back(gb);
	}

	// koopa 
	for (const auto& kPos : config->entityData.koopaStarts)
	{
		const auto kp = new Koopa(kPos.x, kPos.y);
		objects.push_back(kp);
	}

	// Winged koopa
	for (const auto& fkPos : config->entityData.WingedKoopaStarts)
	{
		const auto fkp = new Koopa(fkPos.x, fkPos.y, KoopaForm::Winged);
		objects.push_back(fkp);
	}

	// Bowser
	if (config->entityData.bowserStart.hasValue)
	{
		auto pos = config->entityData.bowserStart.value;
		const auto bowser = new Bowser(pos.x, pos.y, sceneContext->mario);
		objects.push_back(bowser);
	}

	// FireShooter
	for (const auto& fsPos : config->entityData.fireShooters)
	{
		const auto fs = new FireShooter(fsPos.position.x, fsPos.position.y, fsPos.shootDirection);
		objects.push_back(fs);
	}

	// question

	for (const auto& qbData : config->entityData.questionBlocks)
	{
		const auto qb = new QuestionBlock(qbData.position, qbData.dropType, config->biome, false);
		objects.push_back(qb);
	}

	//bricks
	for (const auto& qbData : config->entityData.brickBlocks)
	{
		const auto qb = new QuestionBlock(qbData.position, qbData.dropType, config->biome, true, qbData.isHidden);
		objects.push_back(qb);
	}


	// coins
	for (const auto& cPos : config->entityData.coins)
	{
		const auto coin = new Coin(cPos, config->biome);
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


	// pipes
	for (const auto& pipeData : config->entityData.pipes)
	{
		const auto pipe = new Pipe(pipeData);
		objects.push_back(pipe);
	}
	// music triggers
	for (const auto& musicTriggerData : config->entityData.musicTriggers)
	{
		const auto musicTrigger = new BgMusicTrigger(musicTriggerData.id, musicTriggerData.zone);
		objects.push_back(musicTrigger);
	}

	// bridge
	if (config->entityData.bridge.hasValue)
	{
		const auto bridgeData = config->entityData.bridge.value;
		const auto bridge = new Bridge(bridgeData);
		objects.push_back(bridge);
		const auto axe = new AxeBridge(bridgeData.axePosition, bridge);
		objects.push_back(axe);
	}

	// mario in water trigger
	for (const auto& waterTrigger : config->entityData.waterTriggers)
	{
		const auto trigger = new InWaterTrigger(waterTrigger.zone, waterTrigger.inWater);
		objects.push_back(trigger);
	}

	// background music
	if (config->entityData.backgroundMusicID.hasValue)
	{
		AudioManager::GetInstance()->PlayMusic(config->entityData.backgroundMusicID.value);
	}
	levelTimer = new Timer(timeLeftForLevel);
	levelTimer->Start();

	// background color
	Game::GetInstance()->SetBackgroundColor(config->backgroundColor);
	// triggers;
	for (const auto& colorTriggerData : config->entityData.clearScreenColorTriggers)
	{
		const auto colorTrigger = new ClearScreenColorTrigger(colorTriggerData.zone, colorTriggerData.color);
		objects.push_back(colorTrigger);
	}
	for (const auto& fPos : config->entityData.fireShooters)
	{
		const auto fs = new FireShooter(fPos.position.x, fPos.position.y, fPos.shootDirection);
		objects.push_back(fs);
	}
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

	while (!addPendingGos.empty())
	{
		delete addPendingGos.front();
		addPendingGos.pop();
	}

	delete sceneContext;
	sceneContext = nullptr;
}

void PlayableScene::Render()
{
	render_queue renderQueue;
	const auto tileMap = LevelLoader::GetInstance()->GetTilemapForLevel(level);


	renderQueue.emplace(tileMap->GetRenderIndex(), [&tileMap]
		{
			tileMap->Render();
		});

	for (const auto& obj : objects)
	{
		if (!obj->IsActive())
			continue;

		renderQueue.emplace(obj->GetRenderIndex(), [&obj]
			{
				obj->Render();
			});
	}

	while (!renderQueue.empty())
	{
		auto& obj = renderQueue.top();
		obj.second();
		renderQueue.pop();
	}
	HUD::GetInstance()->Render();
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
