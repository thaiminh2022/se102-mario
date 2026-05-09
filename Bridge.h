#pragma once
#include "GameObject.h"
#include "Rect.h"
#include "Vector2.h"
#include "Timer.h"

enum class BridgeState
{
	Normal,
	Disappearing,
	Disappeared,
};

class Bridge : public GameObject
{
	BridgeData bridgeData;
	BridgeState state;
	Timer disappearTimer;

public:
	explicit Bridge(const BridgeData &bridgeData);
	void SetState(BridgeState newState);
	Rect GetBoundingBox() override;
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;	
};


class AxeBridge : public GameObject
{
	Bridge* source;
public:
	explicit AxeBridge(const Vector2Int& axePosition, Bridge* source);
	Rect GetBoundingBox() override
	{
		return Rect::FromXYWH(position.x, position.y, 16, 16);
	}
	void SolveCollisionWithMario();
	void Render() override;
};