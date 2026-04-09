#include "NextLevelPortal.h"

#include "Game.h"

// We still change the position for thematic things i guess
NextLevelPortal::NextLevelPortal(Rect zone, int levelToLoad) : GameObject(static_cast<float>(zone.top), static_cast<float>(zone.left))
{
	this->zone = zone;
	this->levelToLoad = levelToLoad;
}

void NextLevelPortal::RequestNextLevel() const
{
	Game::GetInstance()->IndicateSceneSwitch(levelToLoad);
}




