#include "MarioBreathBubble.h"
#include "AssetIDs.h"
#include "Collision.h"
#include "CollisionEvent.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"

MarioBreathBubble::MarioBreathBubble(Vector2 pos) : GameObject(pos)
{
	const auto t = Textures::GetInstance()->Get(MARIO_TEX_ID);
	Sprites::GetInstance()->Add(MARIO_BREATH_BUBBLE, 48, 48, 51, 51, t);
}

void MarioBreathBubble::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	constexpr float speed = -30.0f;
	velocity.y = speed;

	Collision::GetInstance()->ProcessCollision(this, coObjects, ctx->tilemap, dt);
}

void MarioBreathBubble::Render()
{
	float renderX, renderY;
	Game::GetInstance()
	->GetCamera()
	->WorldToScreen(position.x, position.y, renderX, renderY);

	Sprites::GetInstance()
	->Get(MARIO_BREATH_BUBBLE)
	->Draw(round(renderX), round(renderY), false, false);
}

bool MarioBreathBubble::IsBlocking()
{
	return false;
}

void MarioBreathBubble::OnNoCollision(float dt)
{
	position += velocity * dt;
}

void MarioBreathBubble::OnCollisionWith(CollisionEvent* event)
{
	isDeleted = true;
}

Rect MarioBreathBubble::GetBoundingBox()
{
	return Rect::FromXYWH(position.x, position.y, 8, 8);
}

CollisionMatrixLayer MarioBreathBubble::GetCollisionLayer()
{
	// just need something that doesn't collide with unwanted shits
	return CollisionMatrixLayer::Player;
}
