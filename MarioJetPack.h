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
	MarioJetPackState state;

public:

	Rect GetBoundingBox() override;
	MarioJetPack(Vector2Int startPos);
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void SetState(MarioJetPackState s);
	void Render() override;
	bool IsBlocking() override;
	bool IsActive() override { return true; }
};

