#include "NextLevelPortal.h"

#include "Game.h"
#include "Mario.h"
#include "PlayableScene.h"


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
		auto scene = dynamic_cast<PlayableScene*>(Game::GetInstance()->GetCurrentScene());
		if (scene != nullptr && !scene->IsReadyForLevelTransition())
		{
			return;
		}
		Game::GetInstance()->IndicateSceneSwitch(levelToLoad, SceneSwitchContext::NormalTransition(ctx->mario->GetPowerLevel()));
	}
}




