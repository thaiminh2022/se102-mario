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

			auto marioPower = ctx->mario->GetPowerLevel();
			if (marioPower == MarioPower::StarmanBig)
			{
				marioPower = MarioPower::Big;
			}
			else if (marioPower == MarioPower::StarmanSmall)
			{
				marioPower = MarioPower::Normal;
			}

			if (pipeData.returnPipeData.has_value())
			{
				auto returnData = pipeData.returnPipeData.value();
				auto marioPipeCtx = MarioPipeCtx{
					.dir = returnData.returnDirection,
					.returnZone = returnData.returnRect,
					.moveTo = returnData.moveTo,
				};
				Optional switchCtx = SceneSwitchContext::PipeTransition(marioPipeCtx, marioPower);

				Game::GetInstance()
					->IndicateSceneSwitch(pipeData.nextLevelToLoad.value(), switchCtx);
			}
			else
			{
				auto switchCtx = SceneSwitchContext::NormalTransition(marioPower);
				Game::GetInstance()
					->IndicateSceneSwitch(pipeData.nextLevelToLoad.value(), switchCtx);
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
					.dir = returnData.returnDirection,
					.returnZone = returnData.returnRect,
					.moveTo = returnData.moveTo,
				};

				ctx->mario->SetExitPipe(marioPipeCtx);
			}
		}

	}
	
}
