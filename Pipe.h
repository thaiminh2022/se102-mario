#pragma once
#include "GameObject.h"
#include "Timer.h"


enum class PipeState
{
	Idle,
	Transition,
	Blocked,
};

class Pipe : public GameObject
{
	PipeData pipeData;
	PipeState pipeState;
	Timer transitionTimer;
public:
	Pipe(const PipeData& pData);

	Rect GetBoundingBox() override;
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	bool IsCollidable() override { return false; }
	bool IsBlocking() override { return false; }
};

