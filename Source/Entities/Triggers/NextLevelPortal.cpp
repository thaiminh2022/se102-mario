#include "NextLevelPortal.h"

#include "Game.h"
#include "Mario.h"


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

		auto power = ctx->mario->GetPowerLevel();
		if (power == MarioPower::StarmanBig)
		{
			power = MarioPower::Big;
		}
		else if (power == MarioPower::StarmanSmall)
		{
			power = MarioPower::Normal;
		}

		auto sceneCtx = SceneSwitchContext::NormalTransition(power);
		Game::GetInstance()->IndicateSceneSwitch(levelToLoad, sceneCtx);
	}
}




