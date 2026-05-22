#pragma once
#include "GameObject.h"

enum class MarioJetPackState
{
	Idle,
	OnMario,
	Removed,
};

class MarioJetPack : public GameObject
{
	float pMeter = 0;
	bool readyToFly = false;
	MarioJetPackState state;

public:

	Rect GetBoundingBox() override;
	explicit MarioJetPack(Vector2Int startPos);
	bool ReadyToFly() const
	{
		return pMeter > 0 && readyToFly;
	}
	void UpdateMeter(float dt, bool canCharge);
	void DrainFlight(float dt);
	float GetPMeter() const { return pMeter; }
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void SetState(MarioJetPackState s);
	void Render() override;
	void RenderCrownAt(const Vector2& marioPosition) const;
	bool IsBlocking() override;
	bool IsActive() override { return true; }
};

