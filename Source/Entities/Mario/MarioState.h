#pragma once

#include <cstdint>
#include <memory>
#include <vector>

struct CollisionEvent;
class GameObject;
class Mario;
struct SceneContext;

enum class MarioState : std::uint8_t
{
	Idle,
	Walking,
	Running,
	Skidding,
	Jumping,
	Ducking,
	PullingFlag,
	WalkingToCastle,
	EnteringPipe,
	ExitingPipe,
	Dying,
	Firing,
	Growing,
	Shrinking,
	StopToWaitBowser,
	Moving,
	Swimming,
};

class MarioStateBase
{
protected:
	Mario& mario;

public:
	explicit MarioStateBase(Mario& mario) : mario(mario) {}
	virtual ~MarioStateBase() = default;

	virtual MarioState GetState() const = 0;
	virtual bool CanRepresent(MarioState state) const { return GetState() == state; }
	virtual void EnterState() {}
	virtual void ExitState() {}
	virtual void Update(float dt, std::vector<GameObject*>& coObjects, SceneContext* ctx);
	virtual void OnCollisionWith(CollisionEvent* event);
	virtual void OnNoCollision(float dt);
};

class MarioStateMachine
{
	Mario* mario;
	std::unique_ptr<MarioStateBase> currentState;

	std::unique_ptr<MarioStateBase> CreateState(MarioState newState) const;

public:
	explicit MarioStateMachine(Mario* mario, MarioState initialState = MarioState::Idle);

	MarioState GetState() const;
	void ChangeState(MarioState newState);
	void Update(float dt, std::vector<GameObject*>& coObjects, SceneContext* ctx);
	void OnCollisionWith(CollisionEvent* event);
	void OnNoCollision(float dt);
};
