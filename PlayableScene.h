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
	float timeLeftForLevel = 400; //time units w/ each = ~0.4s
	bool isFlagPoleSequenceStarted;
	bool isTimeScoreCounting;
	bool isStageClearMusicFinished;
	bool isCastleFlagSequenceStarted;
	bool isCastleFlagSequenceFinished;
	bool isCastleFlagTransitionDelayFinished;
	bool isFireworkSequenceStarted;
	bool isFireworkSequenceFinished;
	int timeScoreStartValue; //the time left when player hit the flag pole
	int timeScoreAwardedUnits; // how many time score units HAVE BEEN AWARDED so far
	int displayTimeLeft; // the time left to display on screen, will count down to 0 as time score is awarded
	int fireworkCount;
	int fireworksRemaining;
	int nextFireworkPositionIndex;
	unsigned int timeScoreAudioHandle;
	float timeScoreElapsed; // how much time has elapsed since time score countdown started, used to calculate how many time score units to award
	float timeScoreDuration; // how long the time score countdown should last, used to calculate how many time score units to award
	float castleFlagTransitionDelayTimer;
	float fireworkSpawnTimer;
	float fireworkFinishTimer;
	vector<Vector2Int> fireworkPositions;
	CastleFlag* castleFlag;

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
		levelTimer = nullptr;
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
		return levelTimer->GetTimeLeft();
	}
	Timer* GetLevelTimer() {
		return levelTimer;
	}
};