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
	if (pipeData.isReturnPipe)
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
				AudioManager::GetInstance()->PauseMusic();
				AudioManager::GetInstance()->PlaySFX(PIPE_ENTER);
			}
		}else
		{
			ctx->mario->SetEnterPipe(pipeData);
			transitionTimer.Start();
			pipeState = PipeState::Transition;
			AudioManager::GetInstance()->PauseMusic();
			AudioManager::GetInstance()->PlaySFX(PIPE_ENTER);
		}
	}
	else if (pipeState == PipeState::Transition)
	{
		transitionTimer.ProcessTimer(dt);
		if (!transitionTimer.IsFinished())
			return;


		if (!pipeData.isTeleportPipe)
		{
			if (!pipeData.nextLevelToLoad.has_value())
				return;

			if (pipeData.returnPipeData.has_value())
			{
				auto returnData = pipeData.returnPipeData.value();
				auto marioPipeCtx = MarioPipeCtx{
					returnData.returnDirection,
					returnData.returnRect,
					returnData.moveTo,
				};
				Optional<SceneSwitchContext> switchCtx = SceneSwitchContext::PipeTransition(marioPipeCtx, ctx->mario->GetPowerLevel());

				Game::GetInstance()
					->IndicateSceneSwitch(pipeData.nextLevelToLoad.value(), switchCtx);
			}
			else
			{
				Game::GetInstance()
					->IndicateSceneSwitch(pipeData.nextLevelToLoad.value(), SceneSwitchContext::NormalTransition(ctx->mario->GetPowerLevel()));
			}
		}else
		{
			if (pipeData.teleportToPosition.has_value())
			{
				Game::GetInstance()->GetCamera()->SetPosition(0, 0);
				ctx->mario->SetPosition(pipeData.teleportToPosition.value());
				ctx->mario->ResetRender();
				ctx->mario->ResetState();
				pipeState = PipeState::Blocked;
			}
			else if (pipeData.returnPipeData.has_value())
			{
				auto returnData = pipeData.returnPipeData.value();
				auto marioPipeCtx = MarioPipeCtx{
					returnData.returnDirection,
					returnData.returnRect,
					returnData.moveTo,
				};

				ctx->mario->SetExitPipe(marioPipeCtx);
			}
		}

	}
	
}
