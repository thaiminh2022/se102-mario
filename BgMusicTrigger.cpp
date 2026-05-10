#include "BgMusicTrigger.h"

#include "AudioManager.h"
#include "Mario.h"

BgMusicTrigger::BgMusicTrigger(int musicId, Rect zone)
{
	this->musicId = musicId;
	this->zone = zone;
	triggered = false;
}


Rect BgMusicTrigger::GetBoundingBox()
{
	return zone;
}

void BgMusicTrigger::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (triggered)
		return;

	if (ctx->mario->GetBoundingBox().IsColliding(GetBoundingBox()))
	{
		AudioManager::GetInstance()
		->PlayMusic(musicId);
		triggered = true;

		// delete this cuz this only get use once lol
		isDeleted = true;
	}
}
