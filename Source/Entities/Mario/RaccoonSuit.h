#pragma once
#include "GameObject.h"

enum class RaccoonSuitState
{
	Idle,
	OnMario,
	Removed,
};
const float PMETER_MAX = 100.0f;
const float PMETER_CHARGE_RATE = 100.0f;
const float PMETER_DECAY_RATE = 55.0f; // the pmeter will become 0 after PMETER_MAX / PMETER_DECAY_RATE seconds of not charging
const float PMETER_FLIGHT_DRAIN_RATE = 5.0f;

class RaccoonSuit : public GameObject
{
	float pMeter = 0;
	bool readyToFly = false;
	RaccoonSuitState state;

public:

	Rect GetBoundingBox() override;
	explicit RaccoonSuit(Vector2Int startPos);
	bool ReadyToFly() const
	{
		return pMeter >= PMETER_MAX && readyToFly;
	}
	void UpdateMeter(float dt, bool canCharge);
	void DrainFlight(float dt);

	float GetPMeter() const { return pMeter; }
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void SetState(RaccoonSuitState s);
	void Render() override;
	void RenderCrownAt(const Vector2& marioPosition) const;
	bool IsBlocking() override;
	bool IsActive() override { return true; }
};

