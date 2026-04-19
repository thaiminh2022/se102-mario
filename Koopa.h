#pragma once
#include "GameObject.h"
#include "Rect.h"
#include "Scene.h"
#include <vector>
#include "Timer.h"

const float KOOPA_JUMP_SPEED = 180.0f;	

enum class KoopaState : std::uint8_t
{
	Moving,
	NotMoving,
	Dead,
	DeadUpsideDown
};

enum class KoopaForm : std::uint8_t
{
	Normal,
	Flying,
	HiddingInShell
};

class Koopa : public GameObject
{
	bool moveLeft;
	KoopaState state;
	KoopaForm form;
	Timer deadTimer;
	float fallAcc = 562.5f;
public:
	Koopa(int startX, int startY);
	Koopa(int startX, int startY, KoopaForm form);
	void SetState(KoopaState newState);
	KoopaState GetState() const { return state; }
	void SetForm(KoopaForm newForm);
	KoopaForm GetForm() const { return form; }
	void Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx) override;
	void Render() override;
	void OnNoCollision(float dt) override;
	void OnCollisionWith(CollisionEvent* event) override;
	Rect GetBoundingBox() override
	{
		return Rect::FromXYWH(static_cast<int>(position.x), static_cast<int>(position.y), 16, 24);
	}
	bool IsBlocking() override
	{
		return false;
	}
};

