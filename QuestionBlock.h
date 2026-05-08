#pragma once
#include "GameObject.h"
#include "Timer.h"

enum class QuestionBlockState
{
	Opened,
	Blocked,
	Closed,
	Break,
};

class QuestionBlock : public GameObject
{
	QuestionBlockState state;
	BlockDropType drop;
	Vector2 startPosition;
	Vector2 renderPosition;
	Timer moveUpTimer;

	bool spawnInternalItem;
	Rect bounceCheckBox;

	bool isBrick; // there's no differences for now
	bool isHidden;

public:
	void SetState(QuestionBlockState newState);
	bool HaveDrop() const { return drop != BlockDropType::None; }

	void Render() override;
	void CheckHitBounce(vector<GameObject*>& coObjects, SceneContext* ctx) const;
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	QuestionBlock(Vector2Int startPos, BlockDropType drop, bool isBrick = false, bool isHidden = false);
	Rect GetBoundingBox() override;
	bool IsCollidable() override { return true; }
	bool IsBlocking() override { return true; }
};
