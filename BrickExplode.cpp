#include "BrickExplode.h"

#include "AssetIDs.h"
#include "AudioManager.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"

BrickExplode::BrickExplode(Vector2 startPosition) : GameObject(startPosition.x, startPosition.y)
{
	auto t = Textures::GetInstance()->Get(BLOCKS_OVERWORLD_TEX_ID);
	auto sp = Sprites::GetInstance();

	sp->Add(BRICK_EXPLODE_SPRITE_1, 32, 16, 39, 22, t);
	sp->Add(BRICK_EXPLODE_SPRITE_2, 40, 16, 47, 22, t);
	sp->Add(BRICK_EXPLODE_SPRITE_3, 32, 24, 39, 31, t);
	sp->Add(BRICK_EXPLODE_SPRITE_4, 40, 24, 47, 31, t);
	

	flipState = false;

	lifeTimer = Timer(2.0f);
	lifeTimer.Start();
	spinTimer = Timer(0.1f);
	spinTimer.Start();


	topLeftDebris = position; // top left debris
	topRightDebris = position + Vector2(8, 0); // top right debris
	bottomLeftDebris = position + Vector2(0, 8); // bottom left debris
	bottomRightDebris = position + Vector2(8, 8); // bottom right debris

	velTopLeft = Vector2(-BRICK_EXPLODE_SIDE_SPEED, BRICK_EXPLODE_JUMP);
	velTopRight = Vector2(BRICK_EXPLODE_SIDE_SPEED, BRICK_EXPLODE_JUMP);
	velBottomLeft = Vector2(-BRICK_EXPLODE_SIDE_SPEED, BRICK_EXPLODE_JUMP);
	velBottomRight = Vector2(BRICK_EXPLODE_SIDE_SPEED, BRICK_EXPLODE_JUMP);

	AudioManager::GetInstance()->PlaySFX(BREAK_BLOCK);
}

void BrickExplode::Update(float dt, vector<GameObject*>& coObjects, SceneContext* ctx)
{
	lifeTimer.ProcessTimer(dt);
	if (lifeTimer.IsFinished())
	{
		isDeleted = true;
		lifeTimer.SetIdle();
		return;
	}
	if (isDeleted)
		return;

	velTopLeft.y += BRICK_GRAVITY  * dt;
	velTopRight.y += BRICK_GRAVITY  * dt;
	velBottomLeft.y += BRICK_GRAVITY  * dt;
	velBottomRight.y += BRICK_GRAVITY * dt;
	
	
	topLeftDebris += velTopLeft * dt;
	topRightDebris += velTopRight * dt;
	bottomLeftDebris += velBottomLeft * dt;
	bottomRightDebris += velBottomRight * dt;

	// spinning
	spinTimer.ProcessTimer(dt);
	if (spinTimer.IsFinished())
	{
		flipState = !flipState;
		spinTimer.Start();
	}

}

void BrickExplode::Render()
{
	auto sp = Sprites::GetInstance();
	auto cam = Game::GetInstance()->GetCamera();
	float renderX, renderY;

	cam->WorldToScreen(topLeftDebris.x, topLeftDebris.y, renderX, renderY);
	sp->Get(BRICK_EXPLODE_SPRITE_1)->Draw(round(renderX), round(renderY), flipState, flipState);
	
	cam->WorldToScreen(topRightDebris.x, topRightDebris.y, renderX, renderY);
	sp->Get(BRICK_EXPLODE_SPRITE_2)->Draw(round(renderX), round(renderY), flipState, flipState);

	cam->WorldToScreen(bottomLeftDebris.x, bottomLeftDebris.y, renderX, renderY);
	sp->Get(BRICK_EXPLODE_SPRITE_3)->Draw(round(renderX), round(renderY), flipState, flipState);
	
	cam->WorldToScreen(bottomRightDebris.x, bottomRightDebris.y, renderX, renderY);
	sp->Get(BRICK_EXPLODE_SPRITE_4)->Draw(round(renderX), round(renderY), flipState, flipState);


}


