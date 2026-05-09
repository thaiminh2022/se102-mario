#include "FireShooter.h"
#include "Mario.h"
#include "Game.h"
#include "BowserFireBullet.h"

FireShooter::FireShooter(int startX, int startY, Vector2Int direction) : GameObject(static_cast<float>(startX), static_cast<float>(startY))
{
	this->isFacingRight = direction == Vector2Int::Right();
	velocity.x = 0.0f;
	velocity.y = 0.0f;
	isShooting = false;
	shootTimer = Timer(1.0f);
	blocksDistanceToShoot = 27;
}

void FireShooter::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	if (!isShooting) {
		auto mario = ctx->mario;
		if (mario != nullptr) {
			float dist = position.Distance(mario->position);
			if (dist <= blocksDistanceToShoot * 16.0)
			{
				isShooting = true;
				shootTimer.Start();
				return;
			}
		}
	}
	else {
		auto r = GetBoundingBox();
		bool isInView = Game::GetInstance()->GetCamera()->IsInView(r.left, r.top, r.right, r.bottom);
		if (isInView) {
			isDeleted = true;
			return;
		}
		shootTimer.ProcessTimer(dt);
		if (shootTimer.IsFinished())
		{
			auto f = new BowserFireBullet(position.x, position.y, isFacingRight, this->position.y, true, true);
			ctx->addObject(f);
			shootTimer.Start();
		}
	}

}

void FireShooter::Render()
{
}

void FireShooter::OnNoCollision(float dt)
{
}

void FireShooter::OnCollisionWith(CollisionEvent* event)
{
}
