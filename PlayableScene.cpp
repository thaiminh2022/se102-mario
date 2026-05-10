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
#include "CastleFlag.h"
#include "Coin.h"
#include "Firework.h"
#include "FireballTrap.h"
#include "FlagPole.h"
#include "Goomba.h"
#include "Koopa.h"
#include "NextLevelPortal.h"
#include "PointPopup.h"
#include "Pipe.h"
#include "QuestionBlock.h"
#include "HUD.h"
#include "StatManager.h"
#include <queue>


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

constexpr int TIME_SCORE_POINTS = 50;
constexpr float FIREWORK_SPAWN_INTERVAL = 0.45f;
constexpr float FIREWORK_FINISH_DELAY = 0.65f;
constexpr float STAGE_CLEAR_FALLBACK_DURATION = 5.6f;
constexpr float CASTLE_FLAG_TRANSITION_DELAY = 2.0f;

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
	if (isFlagPoleSequenceStarted)
	{
		UpdateTimeScore(dt);
		UpdateCastleFlag(dt);
		UpdateFireworks(dt);
	}
	else
	{
		const bool isMarioDying = sceneContext != nullptr
			&& sceneContext->mario != nullptr
			&& sceneContext->mario->GetState() == MarioState::Dying;
		const bool isMarioEnteringPipe = sceneContext != nullptr
			&& sceneContext->mario != nullptr
			&& sceneContext->mario->GetState() == MarioState::EnteringPipe;
		if (!isMarioDying && !isMarioEnteringPipe)
		{
			levelTimer.ProcessTimer(dt * 2.5f);
		}
		displayTimeLeft = static_cast<int>(levelTimer.GetTimeLeft());
		if (displayTimeLeft < 0)
		{
			displayTimeLeft = 0;
		}
		if (levelTimer.IsFinished())
		{
			// Time's up, kill Mario
			//ctx->mario->OnMarioHit();
		}
	}
	HUD::GetInstance()->Update(dt);
	HUD::GetInstance()->GetElement(3)->SetText(L"TIME\n" + std::to_wstring(displayTimeLeft));
}

void PlayableScene::Load(const Optional<SceneSwitchContext>& ctx)
{
	if (sceneContext == nullptr)
	{
		sceneContext = new SceneContext;
	}
	sceneContext->tilemap = LevelLoader::GetInstance()->GetTilemapForLevel(level);
	sceneContext->addObject =[this](GameObject *go)
	{
		AddObject(go);
	};

	auto config = sceneContext->tilemap->GetConfig();
	isFlagPoleSequenceStarted = false;
	isTimeScoreCounting = false;
	isStageClearMusicFinished = false;
	isCastleFlagSequenceStarted = false;
	isCastleFlagSequenceFinished = false;
	isCastleFlagTransitionDelayFinished = false;
	isFireworkSequenceStarted = false;
	isFireworkSequenceFinished = false;
	timeScoreStartValue = 0;
	timeScoreAwardedUnits = 0;
	displayTimeLeft = static_cast<int>(timeLeftForLevel);
	fireworkCount = 0;
	fireworksRemaining = 0;
	nextFireworkPositionIndex = 0;
	timeScoreAudioHandle = 0;
	timeScoreElapsed = 0.0f;
	timeScoreDuration = 0.0f;
	castleFlagTransitionDelayTimer = 0.0f;
	fireworkSpawnTimer = 0.0f;
	fireworkFinishTimer = 0.0f;
	fireworkPositions.clear();
	castleFlag = nullptr;

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
		fireworkPositions = flag.fireworkPositions;
		objects.push_back(new FlagPole(flag.zone, flag.moveToPosition));
	}


	// pipes
	for (const auto& pipeData : config->entityData.pipes)
	{
		const auto pipe = new Pipe(pipeData);
		objects.push_back(pipe);
	}

	// background music
	if (config->entityData.backgroundMusicID.hasValue)
	{
		AudioManager::GetInstance()->PlayMusic(config->entityData.backgroundMusicID.value);
	}
	float startingTimeLeft = timeLeftForLevel;
	if (ctx.hasValue && ctx.value.levelTimeLeft.hasValue)
	{
		startingTimeLeft = ctx.value.levelTimeLeft.value;
	}
	if (startingTimeLeft < 0.0f)
	{
		startingTimeLeft = 0.0f;
	}
	displayTimeLeft = static_cast<int>(startingTimeLeft);
	levelTimer = Timer(startingTimeLeft);
	levelTimer.Start();
	// background color
	Game::GetInstance()->SetBackgroundColor(config->backgroundColor);
}

void PlayableScene::UpdateTimeScore(float dt)
{
	if (!isTimeScoreCounting)
		return;

	if (timeScoreDuration <= 0.0f || timeScoreStartValue <= 0)
	{
		FinishTimeScoreCountdown();
		return;
	}

	timeScoreElapsed += dt;
	float progress = timeScoreElapsed / timeScoreDuration; //percentage of time elapsed in the countdown
	if (progress > 1.0f)
	{
		progress = 1.0f;
	}
	// Calculate how many time score units to award based on the elapsed time
	int targetDisplayTime = timeScoreStartValue - static_cast<int>(progress * static_cast<float>(timeScoreStartValue));
	if (progress >= 1.0f)
	{
		targetDisplayTime = 0;
	}

	const int awardedUnits = timeScoreStartValue - targetDisplayTime;
	const int unitsToAward = awardedUnits - timeScoreAwardedUnits;
	if (unitsToAward > 0)
	{
		StatManager::GetInstance()->AddScore(unitsToAward * TIME_SCORE_POINTS);
		timeScoreAwardedUnits = awardedUnits;
	}

	displayTimeLeft = targetDisplayTime;

	if (progress >= 1.0f)
	{
		FinishTimeScoreCountdown();
	}
}

void PlayableScene::StartTimeScoreCountdown(float duration)
{
	if (timeScoreStartValue <= 0)
	{
		FinishTimeScoreCountdown();
		return;
	}

	if (duration <= 0.0f)
	{
		duration = STAGE_CLEAR_FALLBACK_DURATION;
	}

	isTimeScoreCounting = true;
	timeScoreElapsed = 0.0f;
	timeScoreDuration = duration;
	timeScoreAudioHandle = AudioManager::GetInstance()->Play(TING, true);
}

void PlayableScene::FinishTimeScoreCountdown()
{
	if (timeScoreAudioHandle != 0 && timeScoreAudioHandle != static_cast<unsigned int>(-1))
	{
		AudioManager::GetInstance()->Stop(timeScoreAudioHandle);
		timeScoreAudioHandle = 0;
	}

	const int remainingUnits = timeScoreStartValue - timeScoreAwardedUnits;
	if (remainingUnits > 0)
	{
		StatManager::GetInstance()->AddScore(remainingUnits * TIME_SCORE_POINTS);
	}

	timeScoreAwardedUnits = timeScoreStartValue;
	displayTimeLeft = 0;
	timeScoreElapsed = timeScoreDuration;
	isTimeScoreCounting = false;
}

void PlayableScene::UpdateFireworks(float dt)
{
	if (!isCastleFlagSequenceFinished || isFireworkSequenceFinished)
		return;

	if (!isFireworkSequenceStarted)
	{
		isFireworkSequenceStarted = true;
		fireworksRemaining = fireworkCount;
		fireworkSpawnTimer = 0.0f;
		fireworkFinishTimer = FIREWORK_FINISH_DELAY;

		if (fireworksRemaining <= 0)
		{
			isFireworkSequenceFinished = true;
		}
	}

	if (fireworksRemaining > 0)
	{
		fireworkSpawnTimer -= dt;
		if (fireworkSpawnTimer <= 0.0f)
		{
			AddObject(new Firework(GetFireworkSpawnPosition()));
			fireworksRemaining--;
			fireworkSpawnTimer += FIREWORK_SPAWN_INTERVAL;
			if (fireworksRemaining <= 0)
			{
				fireworkFinishTimer = FIREWORK_FINISH_DELAY;
			}
		}
		return;
	}

	if (fireworkFinishTimer > 0.0f)
	{
		fireworkFinishTimer -= dt;
		if (fireworkFinishTimer <= 0.0f)
		{
			isFireworkSequenceFinished = true;
		}
	}
}

void PlayableScene::UpdateCastleFlag(float dt)
{
	if (!isStageClearMusicFinished)
		return;

	if (!isCastleFlagSequenceStarted)
	{
		isCastleFlagSequenceStarted = true;
		castleFlag = new CastleFlag(GetCastleFlagPosition());
		AddObject(castleFlag);
		return;
	}

	if (!isCastleFlagSequenceFinished)
	{
		if (castleFlag == nullptr || castleFlag->IsFinished())
		{
			isCastleFlagSequenceFinished = true;
			castleFlagTransitionDelayTimer = 0.0f;
		}
		return;
	}

	if (!isCastleFlagTransitionDelayFinished)
	{
		castleFlagTransitionDelayTimer += dt;
		if (castleFlagTransitionDelayTimer >= CASTLE_FLAG_TRANSITION_DELAY)
		{
			isCastleFlagTransitionDelayFinished = true;
		}
	}
}

int PlayableScene::GetFireworkCountForTime(int timeLeft) const
{
	switch (timeLeft % 10)
	{
	case 1:
		return 1;
	case 3:
		return 3;
	case 6:
		return 6;
	default:
		return 0;
	}
}

Vector2 PlayableScene::GetFireworkSpawnPosition()
{
	if (!fireworkPositions.empty())
	{
		auto position = fireworkPositions[nextFireworkPositionIndex % fireworkPositions.size()];
		nextFireworkPositionIndex++;
		return Vector2(position);
	}

	const Vector2 fallbackPositions[] = {
		Vector2(-48.0f, 72.0f),
		Vector2(32.0f, 56.0f),
		Vector2(-16.0f, 96.0f),
		Vector2(56.0f, 80.0f),
		Vector2(-64.0f, 64.0f),
		Vector2(8.0f, 104.0f),
	};

	auto camera = Game::GetInstance()->GetCamera();
	auto fallback = fallbackPositions[nextFireworkPositionIndex % 6];
	float anchorX = camera->GetX() + 224.0f;
	if (sceneContext != nullptr && sceneContext->mario != nullptr)
	{
		anchorX = sceneContext->mario->position.x;
	}
	nextFireworkPositionIndex++;
	float spawnX = anchorX + fallback.x;
	const float minX = camera->GetX() + 24.0f;
	const float maxX = camera->GetX() + Game::GetInstance()->GetBackBufferWidth() - 40.0f;
	if (spawnX < minX)
	{
		spawnX = minX;
	}
	if (spawnX > maxX)
	{
		spawnX = maxX;
	}
	return Vector2(spawnX, camera->GetY() + fallback.y);
}

Vector2 PlayableScene::GetCastleFlagPosition()
{
	auto camera = Game::GetInstance()->GetCamera();
	float flagX = camera->GetX() + 224.0f;
	float flagY = camera->GetY() + 112.0f;

	if (sceneContext != nullptr
		&& sceneContext->tilemap != nullptr
		&& sceneContext->tilemap->GetConfig()->entityData.flagPole.hasValue)
	{
		auto moveTo = sceneContext->tilemap->GetConfig()->entityData.flagPole.value.moveToPosition;
		flagX = static_cast<float>(moveTo.x) + 1.0f;
		flagY = static_cast<float>(moveTo.y) - 78.0f;
	}

	const float minX = camera->GetX() + 24.0f;
	const float maxX = camera->GetX() + Game::GetInstance()->GetBackBufferWidth() - 40.0f;
	if (flagX < minX)
	{
		flagX = minX;
	}
	if (flagX > maxX)
	{
		flagX = maxX;
	}

	const float minY = camera->GetY() + 104.0f;
	const float maxY = camera->GetY() + 136.0f;
	if (flagY < minY)
	{
		flagY = minY;
	}
	if (flagY > maxY)
	{
		flagY = maxY;
	}

	return Vector2(flagX, flagY);
}

void PlayableScene::StartFlagPoleSequence()
{
	if (isFlagPoleSequenceStarted)
		return;

	isFlagPoleSequenceStarted = true;
	isTimeScoreCounting = false;
	isStageClearMusicFinished = false;
	isCastleFlagSequenceStarted = false;
	isCastleFlagSequenceFinished = false;
	isCastleFlagTransitionDelayFinished = false;
	isFireworkSequenceStarted = false;
	isFireworkSequenceFinished = false;
	timeScoreStartValue = static_cast<int>(levelTimer.GetTimeLeft());
	if (timeScoreStartValue < 0)
	{
		timeScoreStartValue = 0;
	}
	timeScoreAwardedUnits = 0;
	displayTimeLeft = timeScoreStartValue;
	fireworkCount = GetFireworkCountForTime(timeScoreStartValue);
	fireworksRemaining = 0;
	nextFireworkPositionIndex = 0;
	timeScoreAudioHandle = 0;
	timeScoreElapsed = 0.0f;
	timeScoreDuration = 0.0f;
	castleFlagTransitionDelayTimer = 0.0f;
	fireworkSpawnTimer = 0.0f;
	fireworkFinishTimer = 0.0f;
	castleFlag = nullptr;
	levelTimer.Pause();
}

void PlayableScene::PlayStageClearMusic()
{
	StartTimeScoreCountdown(AudioManager::GetInstance()->GetDuration(STAGE_CLEAR));

	auto handle = AudioManager::GetInstance()->Play(STAGE_CLEAR, false, []()
		{
			auto scene = dynamic_cast<PlayableScene*>(Game::GetInstance()->GetCurrentScene());
			if (scene != nullptr)
			{
				scene->MarkStageClearMusicFinished();
			}
		});

	if (handle == static_cast<unsigned int>(-1))
	{
		MarkStageClearMusicFinished();
	}
}

void PlayableScene::MarkStageClearMusicFinished()
{
	FinishTimeScoreCountdown();
	isStageClearMusicFinished = true;
}

bool PlayableScene::IsReadyForLevelTransition() const
{
	if (!isFlagPoleSequenceStarted)
		return true;

	return !isTimeScoreCounting
		&& isStageClearMusicFinished
		&& isCastleFlagSequenceFinished
		&& isCastleFlagTransitionDelayFinished
		&& isFireworkSequenceStarted
		&& isFireworkSequenceFinished;
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
