#include "Pipe.h"

#include "AssetIDs.h"
#include "AudioManager.h"
#include "Game.h"
#include "InputManager.h"
#include "Mario.h"

Pipe::Pipe(const PipeData& pData)
{
	transitionTimer = Timer(3);
	pipeState = PipeState::Idle;
	this->pipeData = pData;
	position = Vector2Int(pData.zone.left, pData.zone.top);
}

Rect Pipe::GetBoundingBox()
{
	return pipeData.zone;
}

void Pipe::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (pipeData.isReturnPipe || !pipeData.nextLevelToLoad.hasValue)
		return;
	
	if (pipeState == PipeState::Idle)
	{

		auto marioBox = ctx->mario->GetBoundingBox();
		auto pipeBox = GetBoundingBox();

		if (!marioBox.IsColliding(pipeBox))
			return;

		if (pipeData.enterDirection == Vector2Int::Down())
		{

			if (InputManager::GetInstance()->IsKeyDown('S'))
			{
				// mario state change to piping
				ctx->mario->SetEnterPipe(pipeData);
				transitionTimer.Start();
				pipeState = PipeState::Transition;
				AudioManager::GetInstance()->StopAll();
				AudioManager::GetInstance()->PlaySFX(PIPE_ENTER);
			}
		}else
		{
			ctx->mario->SetEnterPipe(pipeData);
			transitionTimer.Start();
			pipeState = PipeState::Transition;
			AudioManager::GetInstance()->StopAll();
			AudioManager::GetInstance()->PlaySFX(PIPE_ENTER);
		}

		return;
	}

	if (pipeState == PipeState::Transition)
	{
		transitionTimer.ProcessTimer(dt);

		if (transitionTimer.IsFinished())
		{
			Game::GetInstance()
			->IndicateSceneSwitch(pipeData.nextLevelToLoad.value, 
				Optional<SceneSwitchContext>(SceneSwitchContext::UseTransitionScene()));
		}
	}
	
}
