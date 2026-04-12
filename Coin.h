#pragma once
#include "GameObject.h"
#include "Timer.h"

enum class CoinState
{
	Idle,
	Collected,
	CollectedFromQuestionBox,
};

class Coin : public GameObject
{
	CoinState state;
	Timer moveUpTimer;

public:
	void SetState(CoinState s);
	void Render() override;
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	Coin(Vector2Int startPos, CoinState state = CoinState::Idle);
	Rect GetBoundingBox() override;
	bool IsBlocking() override { return false; }
};

