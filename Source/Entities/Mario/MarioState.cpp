#include "MarioState.h"
#include "Mario.h"
#include "InputManager.h"

namespace
{
	class MarioIdleState final : public MarioStateBase
	{
	public:
		using MarioStateBase::MarioStateBase;
		MarioState GetState() const override { return MarioState::Idle; }
		void Update(float dt, std::vector<GameObject*>& coObjects, SceneContext* ctx) override
		{
			mario.ProcessPowerTimers(dt);
			if (mario.IsInWater())
			{
				mario.SetState(MarioState::Swimming);
				return;
			}
			if (!mario.IsGrounded())
			{
				mario.SetState(MarioState::Jumping);
				return;
			}
			mario.UpdateGroundMovement(dt);
			mario.UpdateCommonActions(dt, coObjects, ctx);
			mario.FinishStateUpdate(dt, coObjects, ctx);
		}
	};

	class MarioMovingState final : public MarioStateBase
	{
	public:
		using MarioStateBase::MarioStateBase;

		bool CanRepresent(const MarioState state) const override
		{
			return state == MarioState::Moving || state == MarioState::Walking ||
				state == MarioState::Running || state == MarioState::Skidding;
		}

		MarioState GetState() const override
		{
			const auto input = InputManager::GetInstance();
			if (abs(mario.velocity.x) > MAX_WALK)
			{
				return MarioState::Running;
			}
			if ((mario.velocity.x > 0 && input->IsKeyDown('A')) ||
				(mario.velocity.x < 0 && input->IsKeyDown('D')))
			{
				return MarioState::Skidding;
			}
			return MarioState::Walking;
		}

		void Update(float dt, std::vector<GameObject*>& coObjects, SceneContext* ctx) override
		{
			mario.ProcessPowerTimers(dt);
			if (mario.IsInWater())
			{
				mario.SetState(MarioState::Swimming);
				return;
			}
			if (!mario.IsGrounded())
			{
				mario.SetState(MarioState::Jumping);
				return;
			}
			mario.UpdateGroundMovement(dt);
			mario.UpdateCommonActions(dt, coObjects, ctx);
			mario.FinishStateUpdate(dt, coObjects, ctx);
		}
	};

	class MarioJumpingState final : public MarioStateBase
	{
	public:
		using MarioStateBase::MarioStateBase;
		MarioState GetState() const override { return MarioState::Jumping; }
		void Update(float dt, std::vector<GameObject*>& coObjects, SceneContext* ctx) override
		{
			mario.ProcessPowerTimers(dt);
			if (mario.IsInWater())
			{
				mario.SetState(MarioState::Swimming);
				return;
			}
			if (mario.IsGrounded())
			{
				mario.SetState(MarioState::Idle);
				return;
			}
			mario.UpdateAirMovement(dt);
			mario.UpdateCommonActions(dt, coObjects, ctx);
			mario.FinishStateUpdate(dt, coObjects, ctx);
		}
	};

	class MarioSwimmingState final : public MarioStateBase
	{
	public:
		using MarioStateBase::MarioStateBase;
		MarioState GetState() const override { return MarioState::Swimming; }
		void Update(float dt, std::vector<GameObject*>& coObjects, SceneContext* ctx) override
		{
			mario.ProcessPowerTimers(dt);
			if (!mario.IsInWater())
			{
				mario.SetState(mario.IsGrounded() ? MarioState::Idle : MarioState::Jumping);
				return;
			}
			mario.UpdateSwimmingMovement(dt, ctx);
			mario.UpdateCommonActions(dt, coObjects, ctx);
			mario.FinishStateUpdate(dt, coObjects, ctx);
		}
	};

	class MarioDuckingState final : public MarioStateBase
	{
	public:
		using MarioStateBase::MarioStateBase;
		MarioState GetState() const override { return MarioState::Ducking; }
		void Update(float dt, std::vector<GameObject*>& coObjects, SceneContext* ctx) override
		{
			mario.ProcessPowerTimers(dt);
			if (mario.IsInWater())
			{
				mario.SetState(MarioState::Swimming);
				return;
			}
			if (!mario.IsGrounded())
			{
				mario.SetState(MarioState::Jumping);
				return;
			}
			mario.UpdateGroundMovement(dt);
			mario.UpdateCommonActions(dt, coObjects, ctx);
			mario.FinishStateUpdate(dt, coObjects, ctx);
		}
	};

	class MarioFiringState final : public MarioStateBase
	{
	public:
		using MarioStateBase::MarioStateBase;
		MarioState GetState() const override { return MarioState::Firing; }
		void Update(float dt, std::vector<GameObject*>& coObjects, SceneContext* ctx) override
		{
			mario.ProcessPowerTimers(dt);
			if (mario.IsInWater())
			{
				mario.UpdateSwimmingMovement(dt, ctx);
			}
			else if (mario.IsGrounded())
			{
				mario.UpdateGroundMovement(dt);
			}
			else
			{
				mario.UpdateAirMovement(dt);
			}
			mario.UpdateCommonActions(dt, coObjects, ctx);
			mario.FinishStateUpdate(dt, coObjects, ctx);
		}
	};

	class MarioGrowingState final : public MarioStateBase
	{
	public:
		using MarioStateBase::MarioStateBase;
		MarioState GetState() const override { return MarioState::Growing; }
		void Update(float dt, std::vector<GameObject*>&, SceneContext*) override { mario.UpdateTransformGrow(dt); }
	};

	class MarioShrinkingState final : public MarioStateBase
	{
	public:
		using MarioStateBase::MarioStateBase;
		MarioState GetState() const override { return MarioState::Shrinking; }
		void Update(float dt, std::vector<GameObject*>&, SceneContext*) override { mario.UpdateTransformShrink(dt); }
	};

	class MarioDyingState final : public MarioStateBase
	{
	public:
		using MarioStateBase::MarioStateBase;
		MarioState GetState() const override { return MarioState::Dying; }
		void Update(float dt, std::vector<GameObject*>&, SceneContext*) override { mario.UpdateDying(dt); }
	};

	class MarioPullingFlagState final : public MarioStateBase
	{
	public:
		using MarioStateBase::MarioStateBase;
		MarioState GetState() const override { return MarioState::PullingFlag; }
		void Update(float dt, std::vector<GameObject*>&, SceneContext*) override { mario.UpdatePullingFlag(dt); }
	};

	class MarioWalkingToCastleState final : public MarioStateBase
	{
	public:
		using MarioStateBase::MarioStateBase;
		MarioState GetState() const override { return MarioState::WalkingToCastle; }
		void Update(float dt, std::vector<GameObject*>& coObjects, SceneContext* ctx) override
		{
			mario.UpdateWalkingToCastle(dt, coObjects, ctx);
		}
	};

	class MarioEnteringPipeState final : public MarioStateBase
	{
	public:
		using MarioStateBase::MarioStateBase;
		MarioState GetState() const override { return MarioState::EnteringPipe; }
		void Update(float dt, std::vector<GameObject*>&, SceneContext*) override { mario.UpdateEnteringPipe(dt); }
	};

	class MarioExitingPipeState final : public MarioStateBase
	{
	public:
		using MarioStateBase::MarioStateBase;
		MarioState GetState() const override { return MarioState::ExitingPipe; }
		void Update(float dt, std::vector<GameObject*>&, SceneContext*) override { mario.UpdateExitingPipe(dt); }
	};

	class MarioWaitForBowserState final : public MarioStateBase
	{
	public:
		using MarioStateBase::MarioStateBase;
		MarioState GetState() const override { return MarioState::StopToWaitBowser; }
		void Update(float dt, std::vector<GameObject*>&, SceneContext*) override { mario.UpdateWaitingForBowser(dt); }
	};
}

void MarioStateBase::Update(float dt, std::vector<GameObject*>& coObjects, SceneContext* ctx)
{
	mario.ProcessPowerTimers(dt);
	if (mario.IsInWater())
	{
		mario.UpdateSwimmingMovement(dt, ctx);
	}
	else if (mario.IsGrounded())
	{
		mario.UpdateGroundMovement(dt);
	}
	else
	{
		mario.UpdateAirMovement(dt);
	}
	mario.UpdateCommonActions(dt, coObjects, ctx);
	mario.FinishStateUpdate(dt, coObjects, ctx);
}

void MarioStateBase::OnCollisionWith(CollisionEvent* event)
{
	mario.HandleDefaultCollision(event);
}

void MarioStateBase::OnNoCollision(float dt)
{
	mario.HandleDefaultNoCollision(dt);
}

MarioStateMachine::MarioStateMachine(Mario* mario, const MarioState initialState)
	: mario(mario), currentState(nullptr)
{
	currentState = CreateState(initialState);
	currentState->EnterState();
}

std::unique_ptr<MarioStateBase> MarioStateMachine::CreateState(const MarioState newState) const
{
	switch (newState)
	{
	case MarioState::Idle:
		return std::make_unique<MarioIdleState>(*mario);
	case MarioState::Walking:
	case MarioState::Running:
	case MarioState::Skidding:
	case MarioState::Moving:
		return std::make_unique<MarioMovingState>(*mario);
	case MarioState::Jumping:
		return std::make_unique<MarioJumpingState>(*mario);
	case MarioState::Swimming:
		return std::make_unique<MarioSwimmingState>(*mario);
	case MarioState::Ducking:
		return std::make_unique<MarioDuckingState>(*mario);
	case MarioState::Firing:
		return std::make_unique<MarioFiringState>(*mario);
	case MarioState::Growing:
		return std::make_unique<MarioGrowingState>(*mario);
	case MarioState::Shrinking:
		return std::make_unique<MarioShrinkingState>(*mario);
	case MarioState::Dying:
		return std::make_unique<MarioDyingState>(*mario);
	case MarioState::PullingFlag:
		return std::make_unique<MarioPullingFlagState>(*mario);
	case MarioState::WalkingToCastle:
		return std::make_unique<MarioWalkingToCastleState>(*mario);
	case MarioState::EnteringPipe:
		return std::make_unique<MarioEnteringPipeState>(*mario);
	case MarioState::ExitingPipe:
		return std::make_unique<MarioExitingPipeState>(*mario);
	case MarioState::StopToWaitBowser:
		return std::make_unique<MarioWaitForBowserState>(*mario);
	}
	return std::make_unique<MarioIdleState>(*mario);
}

MarioState MarioStateMachine::GetState() const
{
	return currentState->GetState();
}

void MarioStateMachine::ChangeState(const MarioState newState)
{
	if (currentState->CanRepresent(newState))
	{
		return;
	}

	currentState->ExitState();
	currentState = CreateState(newState);
	currentState->EnterState();
}

void MarioStateMachine::Update(float dt, std::vector<GameObject*>& coObjects, SceneContext* ctx)
{
	currentState->Update(dt, coObjects, ctx);
}

void MarioStateMachine::OnCollisionWith(CollisionEvent* event)
{
	currentState->OnCollisionWith(event);
}

void MarioStateMachine::OnNoCollision(float dt)
{
	currentState->OnNoCollision(dt);
}
