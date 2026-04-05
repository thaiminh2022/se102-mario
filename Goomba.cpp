#include "Collision.h"
#include "GameObject.h"
#include "Goomba.h"
#include "Scene.h"
#include <vector>

Goomba::Goomba(int playerStartX, int playerStartY) : GameObject(playerStartX, playerStartY)
{
	// Load the animations
}

void Goomba::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
}

void Goomba::Render()
{
}

void Goomba::OnNoCollision(float dt)
{
	GameObject::OnNoCollision(dt);
}

void Goomba::OnCollisionWith(CollisionEvent* event)
{
	GameObject::OnCollisionWith(event);
}
