#include "Mario.h"

#include "Animations.h"
#include "AssetIDs.h"
#include "Debug.h"
#include "InputManager.h"
#include "Sprites.h"
#include "Textures.h"

Mario::Mario(float startX, float startY) : GameObject(startX, startY)
{
	auto marioTex = Textures::GetInstance()->Get(MARIO_TEX_ID);
	auto anims = Animations::GetInstance();
	auto sprites = Sprites::GetInstance();

	Animation* anim;

	// sprites
	sprites->Add(MARIO_RUN_SPRITE_1, 16, 0, 31, 15, marioTex);
	sprites->Add(MARIO_RUN_SPRITE_2, 32, 0, 47, 15, marioTex);
	sprites->Add(MARIO_RUN_SPRITE_3, 48, 0, 63, 15, marioTex);
	sprites->Add(MARIO_IDLE_SPRITE_1, 0, 0, 15, 15, marioTex);
	
	// idle anim
	anim = new Animation(300);
	anim->Add(MARIO_IDLE_SPRITE_1);
	anims->Add(MARIO_IDLE_ANIM_ID, anim);

	// walk anim
	anim = new Animation(100);
	anim->Add(MARIO_RUN_SPRITE_1);
	anim->Add(MARIO_RUN_SPRITE_2);
	anim->Add(MARIO_RUN_SPRITE_3);
	anims->Add(MARIO_RUN_ANIM_ID, anim);
}

void Mario::Update(DWORD dt, vector<GameObject*>* coObjects)
{
	auto input = InputManager::GetInstance();
	auto dtSec = dt / 1000.0f;
	if (input->IsKeyDown('A'))
	{
		vx = -100.0f;
		state = MarioState::Running;
	}
	else if (input->IsKeyDown('D'))
	{
		vx = 100.0f;
		state = MarioState::Running;

	}else
	{
		vx = 0;
		state = MarioState::Idle;
	}
	x += vx * dtSec;

}

void Mario::Render()
{
	switch (state)
	{
	case MarioState::Running:
		Animations::GetInstance()->Get(MARIO_RUN_ANIM_ID)->Render(round(x), round(y));
		break;
	case MarioState::Idle:
		Animations::GetInstance()->Get(MARIO_IDLE_ANIM_ID)->Render(round(x), round(y));
		break;
	}
}
