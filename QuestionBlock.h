#pragma once
#include "GameObject.h"

enum class QuestionBlockState
{
	Opened,
	Blocked,
	Closed,
};

class QuestionBlock : public GameObject
{
	QuestionBlockState state;
	BlockDropType drop;
	Vector2 startPosition;

public:
	void SetState(QuestionBlockState newState);
	void Render() override;
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;	
	QuestionBlock(Vector2Int startPos, BlockDropType drop);
	Rect GetBoundingBox() override;
	bool IsCollidable() override { return true; }
	bool IsBlocking() override { return true; }

	void OnNoCollision(float dt) override;
};

