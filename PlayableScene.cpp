#include "Game.h"
#include "GameObject.h"
#include "LevelLoader.h"
#include "Mario.h"
#include "PlayableScene.h"
#include "Scene.h"
#include <algorithm>
#include <memory>
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
#include "QuestionBlock.h"
#include "HUD.h"
#include <queue>
#include "Bowser.h"

#include "BgMusicTrigger.h"
#include "CheepCheeps.h"
#include "ClearScreenColorTrigger.h"
#include "EnterCastleTrigger.h"
#include "InWaterTrigger.h"
#include "FireShooter.h"
#include "LevelTextRender.h"
#include "MarioJetPack.h"


using std::priority_queue;
using std::pair;
using std::function;

using RenderItem = pair<int, function<void()>>;


struct RenderCompare
{
	bool operator()(const RenderItem& a, const RenderItem& b) const
	{
		return a.first > b.first;
	}
};

using RenderQueue = priority_queue<RenderItem, vector<RenderItem>, RenderCompare>;

void PlayableScene::Update(float dt)
{
	// coObjects is all object, not including obj itself
	vector<GameObject*> activeCollidableObjects;
	activeCollidableObjects.reserve(objects.size());

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

		if (obj->IsActive() && obj->IsCollidable() && !GameObject::IsDeleted(obj.get()))
		{
			activeCollidableObjects.push_back(obj.get());
		}
	}

	vector<GameObject*> coObjects;
	for (const auto& obj : objects)
	{
		if (!obj->IsActive())
		{
			continue;
		}

		// make co-objects
		coObjects.clear();
		if (obj->IsCollidable())
		{
			for (auto other : activeCollidableObjects)
			{
				if (other == obj.get()) continue;
				if (!CollisionMatrix::IsLayerCollide(other->GetCollisionLayer(), obj->GetCollisionLayer())) continue;
				coObjects.push_back(other);
			}
		}

		obj->Update(dt, coObjects, sceneContext.get());
	}
	Game::GetInstance()->GetCamera()->Update();
	CleanupDeletedObjects();

	while (!addPendingGos.empty())
	{
		auto g = std::move(addPendingGos.front());
		objects.push_back(std::move(g));
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
		sceneContext = std::make_unique<SceneContext>();
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
	auto mario = std::make_unique<Mario>(playerStart.x, playerStart.y);
	sceneContext->mario = mario.get();
	objects.push_back(std::move(mario));
	if (ctx.has_value())
	{
		sceneContext->mario->SetPowerLevel(ctx.value().marioPower);
		if (ctx.value().marioCtx.has_value())
		{
			sceneContext->mario->SetExitPipe(ctx.value().marioCtx.value());
		}
	}

	c->SetTarget(sceneContext->mario);

	// goomba
	for (const auto& gPos : config->entityData.goombaStarts)
	{
		objects.push_back(std::make_unique<Goomba>(gPos, config->biome));
	}

	// koopa 
	for (const auto& kPos : config->entityData.koopaStarts)
	{
		objects.push_back(std::make_unique<Koopa>(kPos, config->biome));
	}

	// Winged koopa
	for (const auto& fkPos : config->entityData.WingedKoopaStarts)
	{
		objects.push_back(std::make_unique<Koopa>(fkPos, config->biome, KoopaForm::Winged));
	}
	//cheep cheeps
	for (const auto& ccData : config->entityData.cheepCheeps)
	{
		objects.push_back(std::make_unique<CheepCheeps>(ccData.startPosition, config->biome, ccData.isRed));
	}
	//bloopers
	for (const auto& bData : config->entityData.bloopers)
	{
		objects.push_back(std::make_unique<Bloopers>(bData.lowestLimit, bData.highestLimit, config->biome));
	}

	// Bowser
	if (config->entityData.bowserStart.has_value())
	{
		const auto bowserStart = config->entityData.bowserStart.value();
		objects.push_back(std::make_unique<Bowser>(bowserStart.x, bowserStart.y, sceneContext->mario));
	}

	// FireShooter
	for (const auto& fsPos : config->entityData.fireShooters)
	{
		objects.push_back(std::make_unique<FireShooter>(fsPos.position.x, fsPos.position.y, fsPos.shootDirection));
	}

	// question
	for (const auto& qbData : config->entityData.questionBlocks)
	{
		objects.push_back(std::make_unique<QuestionBlock>(qbData.position, qbData.dropType, config->biome, false));
	}

	//bricks
	for (const auto& qbData : config->entityData.brickBlocks)
	{
		objects.push_back(std::make_unique<QuestionBlock>(qbData.position, qbData.dropType, config->biome, true, qbData.isHidden));
	}


	// coins
	for (const auto& cPos : config->entityData.coins)
	{
		objects.push_back(std::make_unique<Coin>(cPos, config->biome));
	}

	// jetpack
	for (const auto& cPos : config->entityData.jetpackStart)
	{
		objects.push_back(std::make_unique<MarioJetPack>(cPos));
	}

	// next level portal
	for (const auto& pPos : config->entityData.nextLevelsData)
	{
		objects.push_back(std::make_unique<NextLevelPortal>(pPos.zone, pPos.levelToLoad, pPos.delaySeconds));
	}

	// fire trap
	for (const auto& pPos : config->entityData.fireballTraps)
	{
		objects.push_back(std::make_unique<FireballTrap>(pPos));
	}

	// flagpole
	if (config->entityData.flagPole.has_value())
	{
		const auto flag = config->entityData.flagPole.value();
		objects.push_back(std::make_unique<FlagPole>(flag.zone, flag.moveToPosition));
	}


	// pipes
	for (const auto& pipeData : config->entityData.pipes)
	{
		objects.push_back(std::make_unique<Pipe>(pipeData));
	}

	// text render

	if (!config->entityData.worldTextData.empty())
	{
		objects.push_back(std::make_unique<LevelTextRender>(config->entityData.worldTextData));
	}

	// background music
	if (config->entityData.backgroundMusicID.has_value())
	{
		AudioManager::GetInstance()->PlayMusic(config->entityData.backgroundMusicID.value());
	}
	levelTimer = std::make_unique<Timer>(timeLeftForLevel);
	levelTimer->Start();

	// background color
	Game::GetInstance()->SetBackgroundColor(config->backgroundColor);
	
	// triggers;
	// clear screen color trigger
	for (const auto& colorTriggerData : config->entityData.clearScreenColorTriggers)
	{
		objects.push_back(std::make_unique<ClearScreenColorTrigger>(colorTriggerData.zone, colorTriggerData.color));
	}

	// enter water trigger
	for (const auto& waterTriggerData : config->entityData.waterTriggers)
	{
		objects.push_back(std::make_unique<InWaterTrigger>(waterTriggerData.zone, waterTriggerData.inWater));
	}

	// music trigger
	for (const auto& mData : config->entityData.musicTriggers)
	{
		objects.push_back(std::make_unique<BgMusicTrigger>(mData.id, mData.zone));
	}

	// enter castle trigger
	if (config->entityData.enterCastleTrigger.has_value())
	{
		const auto& data = config->entityData.enterCastleTrigger.value();
		objects.push_back(std::make_unique<EnterCastleTrigger>(data, config->biome));
	}
}

void PlayableScene::UnLoad()
{
	AudioManager::GetInstance()->StopAll();

	objects.clear();

	while (!addPendingGos.empty())
	{
		addPendingGos.pop();
	}

	sceneContext = nullptr;
	levelTimer = nullptr;
}

void PlayableScene::Render()
{
	RenderQueue renderQueue;
	const auto tileMap = LevelLoader::GetInstance()->GetTilemapForLevel(level);


	renderQueue.emplace(tileMap->GetRenderIndex(), [&tileMap]
		{
			tileMap->Render();
		});

	for (const auto& obj : objects)
	{
		if (!obj->IsActive())
			continue;

		auto object = obj.get();
		renderQueue.emplace(object->GetRenderIndex(), [object]
			{
				object->Render();
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
	objects.erase(
		std::remove_if(objects.begin(), objects.end(),
			[](const unique_ptr<GameObject>& o)
			{
				return GameObject::IsDeleted(o.get());
			}),
		objects.end());
}

void PlayableScene::AddObject(GameObject* go)
{
	AddObject(unique_ptr<GameObject>(go));
}

void PlayableScene::AddObject(unique_ptr<GameObject> go)
{
	addPendingGos.push(std::move(go));
}
