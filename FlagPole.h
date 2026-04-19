#pragma once
#include "GameObject.h"


enum class FlagPoleState
{
	Idle,
	Move,
};

class FlagPole : public GameObject
{
	FlagPoleState state;
	Rect zone;
	Vector2Int playerMoveTo;
	Vector2 flagPosition;

public:
	FlagPole(Rect r, Vector2Int moveTo);
	
	Rect GetBoundingBox() override;
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	bool IsBlocking() override { return false; }
	int GetRenderIndex() override { return 1; }


	void SetFlagMove();
	Vector2 GetSnapPosition() const;
	Vector2 GetMoveToPosition();
};

