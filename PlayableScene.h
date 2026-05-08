#pragma once
#include <queue>

#include "Scene.h"
#include <vector>

#include "GameObject.h"
#include "Timer.h"

using std::vector;

class CastleFlag;

class PlayableScene :
	public Scene
{
    std::queue<GameObject*> addPendingGos;

	vector<GameObject*> objects;
	SceneContext* sceneContext;
	int level;

	Timer levelTimer;
	float timeLeftForLevel = 160; //seconds, originally 400 frames w/ each frame ~0.4s
	bool isFlagPoleSequenceStarted = false;
	bool isTimeScoreCounting = false;
	bool isStageClearMusicFinished = false;
	bool isCastleFlagSequenceStarted = false;
	bool isCastleFlagSequenceFinished = false;
	bool isCastleFlagTransitionDelayFinished = false;
	bool isFireworkSequenceStarted = false;
	bool isFireworkSequenceFinished = false;
	int timeScoreStartValue = 0;
	int timeScoreAwardedUnits = 0;
	int displayTimeLeft = 0;
	int fireworkCount = 0;
	int fireworksRemaining = 0;
	int nextFireworkPositionIndex = 0;
	unsigned int timeScoreAudioHandle = 0;
	float timeScoreElapsed = 0.0f;
	float timeScoreDuration = 0.0f;
	float castleFlagTransitionDelayTimer = 0.0f;
	float fireworkSpawnTimer = 0.0f;
	float fireworkFinishTimer = 0.0f;
	vector<Vector2Int> fireworkPositions;
	CastleFlag* castleFlag = nullptr;

	void UpdateTimeScore(float dt);
	void UpdateCastleFlag(float dt);
	void UpdateFireworks(float dt);
	void StartTimeScoreCountdown(float duration);
	void FinishTimeScoreCountdown();
	int GetFireworkCountForTime(int timeLeft) const;
	Vector2 GetCastleFlagPosition();
	Vector2 GetFireworkSpawnPosition();
	
public:
	int maxFireballs = 2; // Max fireballs allowed on screen at once, to prevent spamming
	explicit PlayableScene(const int level)
	{
		this->level = level;
		sceneContext = nullptr; // init on load
	}

    void Update(float dt) override;
    void Load(const Optional<SceneSwitchContext>& ctx) override;
    void UnLoad() override;
	void Render() override;
	void CleanupDeletedObjects();
	void AddObject(GameObject* go);
	void StartFlagPoleSequence();
	void PlayStageClearMusic();
	void MarkStageClearMusicFinished();
	bool IsReadyForLevelTransition() const;

	int GetMaxFireballs() const {
		return maxFireballs;
	}
	float GetTimeLeft() const {
		return levelTimer.GetTimeLeft();
	}
};


