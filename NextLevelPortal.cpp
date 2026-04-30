#include "NextLevelPortal.h"

#include "Game.h"



NextLevelPortal::NextLevelPortal(Rect zone, int levelToLoad, float timeBeforeLoad)
{
	this->zone = zone;
	this->levelToLoad = levelToLoad;
	this->timeBeforeLoad = timeBeforeLoad;
	startCountdown = false;
}

void NextLevelPortal::RequestNextLevel()
{
	startCountdown = true;
}

void NextLevelPortal::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (!startCountdown)
		return;

	if (timeBeforeLoad > 0)
	{
		timeBeforeLoad -= dt;
	}

	if (timeBeforeLoad <= 0)
	{
		Game::GetInstance()->IndicateSceneSwitch(levelToLoad, Optional<SceneSwitchContext>(SceneSwitchContext::UseTransitionScene()));
	}
}




