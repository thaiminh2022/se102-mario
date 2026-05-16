#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "Mario.h"
#include "Sprites.h"
#include "Textures.h"

static void LoadHorizontalSheet(int ids[], int n, int left, int top, int cellWidth, int cellHeight, Texture* t)
{
	const auto sp = Sprites::GetInstance();
	int bottom = top + cellHeight - 1;
	for (int i = 0; i < n; i++)
	{
		const int leftStep = left + cellWidth * i;
		const int right = leftStep + cellWidth - 1;
		sp->Add(ids[i], leftStep, top, right, bottom, t);
	}
}


void Mario::LoadSmallNormalMario()
{
	auto marioTex = Textures::GetInstance()->Get(MARIO_TEX_ID);
	auto anims = Animations::GetInstance();
	auto sprites = Sprites::GetInstance();
	Animation* anim = nullptr;
	// =========================================================
	// NORMAL MARIO (SMALL)
	// =========================================================

	// --- Sprites ---
	sprites->Add(MARIO_IDLE_SPRITE_1, 0, 0, 15, 15, marioTex);
	sprites->Add(MARIO_RUN_SPRITE_1, 16, 0, 31, 15, marioTex);
	sprites->Add(MARIO_RUN_SPRITE_2, 32, 0, 47, 15, marioTex);
	sprites->Add(MARIO_RUN_SPRITE_3, 48, 0, 63, 15, marioTex);
	sprites->Add(MARIO_SKID_SPRITE_1, 64, 0, 79, 15, marioTex);
	sprites->Add(MARIO_JUMP_SPRITE_1, 80, 0, 95, 15, marioTex);
	sprites->Add(MARIO_DEATH_SPRITE_1, 96, 0, 111, 15, marioTex);
	sprites->Add(MARIO_FLAG_PULL_SPRITE_1, 112, 0, 127, 15, marioTex);
	sprites->Add(MARIO_FLAG_PULL_SPRITE_2, 128, 0, 143, 15, marioTex);
	sprites->Add(MARIO_GROWBIG_SPRITE_1, 0, 48, 15, 79, marioTex);
	sprites->Add(MARIO_GROWBIG_SPRITE_2, 16, 48, 31, 79, marioTex);
	sprites->Add(MARIO_GROWBIG_SPRITE_3, 0, 16, 15, 47, marioTex);

	// swim sprites
	int swimIds[4] = { MARIO_SWIM_SPRITE_1, MARIO_SWIM_SPRITE_2, MARIO_SWIM_SPRITE_3, MARIO_SWIM_SPRITE_4 };
	LoadHorizontalSheet(swimIds, 4, 160, 0, 16, 16, marioTex);

	// --- Animations ---
	anim = new Animation(300);
	anim->Add(MARIO_IDLE_SPRITE_1);
	anims->Add(MARIO_IDLE_ANIM_ID, anim);

	anim = new Animation(100);
	anim->Add(MARIO_RUN_SPRITE_1);
	anim->Add(MARIO_RUN_SPRITE_2);
	anim->Add(MARIO_RUN_SPRITE_3);
	anims->Add(MARIO_RUN_ANIM_ID, anim);

	anim = new Animation(100);
	anim->Add(MARIO_SKID_SPRITE_1);
	anims->Add(MARIO_SKID_ANIM_ID, anim);

	anim = new Animation(100);
	anim->Add(MARIO_JUMP_SPRITE_1);
	anims->Add(MARIO_JUMP_ANIM_ID, anim);

	anim = new Animation(100);
	anim->Add(MARIO_DEATH_SPRITE_1);
	anims->Add(MARIO_DEATH_ANIM_ID, anim);

	anim = new Animation(150);
	anim->Add(MARIO_GROWBIG_SPRITE_1, 100);
	anim->Add(MARIO_GROWBIG_SPRITE_2, 100);
	anim->Add(MARIO_GROWBIG_SPRITE_1, 100);
	anim->Add(MARIO_GROWBIG_SPRITE_2, 100);
	anim->Add(MARIO_GROWBIG_SPRITE_3, 100);
	anim->Add(MARIO_GROWBIG_SPRITE_2, 100);
	anim->Add(MARIO_GROWBIG_SPRITE_3, 100);
	anims->Add(MARIO_GROWBIG_ANIM_ID, anim);

	anim = new Animation(150);
	anim->Add(MARIO_FLAG_PULL_SPRITE_1, 150);
	anim->Add(MARIO_FLAG_PULL_SPRITE_2, 150);
	anims->Add(MARIO_FLAG_PULL_ANIM_ID, anim);

	// swim
	anim = new Animation(swimIds, 4, 150);
	anims->Add(MARIO_SWIM_ANIM_ID, anim);
	
}

void Mario::LoadBigNormalMario()
{
	auto marioTex = Textures::GetInstance()->Get(MARIO_TEX_ID);
	auto anims = Animations::GetInstance();
	auto sprites = Sprites::GetInstance();
	Animation* anim;
	// =========================================================
	// BIG MARIO
	// =========================================================

	// --- Sprites ---
	sprites->Add(MARIO_BIG_IDLE_SPRITE_1, 0, 16, 15, 47, marioTex);
	sprites->Add(MARIO_BIG_RUN_SPRITE_1, 16, 16, 31, 47, marioTex);
	sprites->Add(MARIO_BIG_RUN_SPRITE_2, 32, 16, 47, 47, marioTex);
	sprites->Add(MARIO_BIG_RUN_SPRITE_3, 48, 16, 63, 47, marioTex);
	sprites->Add(MARIO_BIG_SKID_SPRITE_1, 64, 16, 79, 47, marioTex);
	sprites->Add(MARIO_BIG_JUMP_SPRITE_1, 80, 16, 95, 47, marioTex);
	sprites->Add(MARIO_BIG_DUCK_SPRITE_1, 96, 16, 111, 47, marioTex);
	sprites->Add(MARIO_SHRINK_SPRITE_1, 32, 48, 47, 79, marioTex);
	sprites->Add(MARIO_SHRINK_SPRITE_2, 48, 48, 63, 79, marioTex);
	sprites->Add(MARIO_SHRINK_SPRITE_3, 64, 48, 79, 79, marioTex);
	sprites->Add(MARIO_BIG_FLAG_PULL_SPRITE_1, 112, 16, 127, 47, marioTex);
	sprites->Add(MARIO_BIG_FLAG_PULL_SPRITE_2, 128, 16, 143, 47, marioTex);

	// swim sprites
	int swimIds[4] = { MARIO_BIG_SWIM_SPRITE_1, MARIO_BIG_SWIM_SPRITE_2, MARIO_BIG_SWIM_SPRITE_3, MARIO_BIG_SWIM_SPRITE_4 };
	LoadHorizontalSheet(swimIds, 4, 160, 16, 16, 32, marioTex);

	// --- Animations ---
	anim = new Animation(100);
	anim->Add(MARIO_BIG_IDLE_SPRITE_1);
	anims->Add(MARIO_BIG_IDLE_ANIM_ID, anim);

	anim = new Animation(100);
	anim->Add(MARIO_BIG_RUN_SPRITE_1);
	anim->Add(MARIO_BIG_RUN_SPRITE_2);
	anim->Add(MARIO_BIG_RUN_SPRITE_3);
	anims->Add(MARIO_BIG_RUN_ANIM_ID, anim);

	anim = new Animation(100);
	anim->Add(MARIO_BIG_SKID_SPRITE_1);
	anims->Add(MARIO_BIG_SKID_ANIM_ID, anim);

	anim = new Animation(100);
	anim->Add(MARIO_BIG_JUMP_SPRITE_1);
	anims->Add(MARIO_BIG_JUMP_ANIM_ID, anim);

	anim = new Animation(100);
	anim->Add(MARIO_BIG_DUCK_SPRITE_1);
	anims->Add(MARIO_BIG_DUCK_ANIM_ID, anim);

	anim = new Animation(100);
	anim->Add(MARIO_SHRINK_SPRITE_1, 50);
	anim->Add(MARIO_SHRINK_SPRITE_3, 50);
	anim->Add(MARIO_SHRINK_SPRITE_1, 50);
	anim->Add(MARIO_SHRINK_SPRITE_3, 50);
	anim->Add(MARIO_SHRINK_SPRITE_1, 50);
	anim->Add(MARIO_SHRINK_SPRITE_3, 50);
	anim->Add(MARIO_SHRINK_SPRITE_1, 50);
	anim->Add(MARIO_SHRINK_SPRITE_3, 50);
	anim->Add(MARIO_SHRINK_SPRITE_2, 50);
	anim->Add(MARIO_SHRINK_SPRITE_3, 50);
	anim->Add(MARIO_SHRINK_SPRITE_2, 50);
	anim->Add(MARIO_SHRINK_SPRITE_3, 50);
	anim->Add(MARIO_SHRINK_SPRITE_2, 50);
	anim->Add(MARIO_SHRINK_SPRITE_3, 50);
	anim->Add(MARIO_SHRINK_SPRITE_2, 50);
	anims->Add(MARIO_SHRINK_ANIM_ID, anim);

	anim = new Animation(150);
	anim->Add(MARIO_BIG_FLAG_PULL_SPRITE_1, 150);
	anim->Add(MARIO_BIG_FLAG_PULL_SPRITE_2, 150);
	anims->Add(MARIO_BIG_FLAG_PULL_ANIM_ID, anim);

	// swim
	anim = new Animation(swimIds, 4, 150);
	anims->Add(MARIO_BIG_SWIM_ANIM_ID, anim);
}

void Mario::LoadFireMario()
{
	auto marioTex = Textures::GetInstance()->Get(MARIO_TEX_ID);
	auto anims = Animations::GetInstance();
	auto sprites = Sprites::GetInstance();
	Animation* anim = nullptr;

	// =========================================================
	// FIRE MARIO
	// =========================================================

	// --- Sprites ---
	sprites->Add(MARIO_FIRE_IDLE_SPRITE_1, 0, 80, 15, 111, marioTex);
	sprites->Add(MARIO_FIRE_RUN_SPRITE_1, 16, 80, 31, 111, marioTex);
	sprites->Add(MARIO_FIRE_RUN_SPRITE_2, 32, 80, 47, 111, marioTex);
	sprites->Add(MARIO_FIRE_RUN_SPRITE_3, 48, 80, 63, 111, marioTex);
	sprites->Add(MARIO_FIRE_SKID_SPRITE_1, 64, 80, 79, 111, marioTex);
	sprites->Add(MARIO_FIRE_JUMP_SPRITE_1, 80, 80, 95, 111, marioTex);
	sprites->Add(MARIO_FIRE_DUCK_SPRITE_1, 96, 80, 111, 111, marioTex);
	sprites->Add(MARIO_FIRE_FIRE_SPRITE_1, 16, 80, 31, 111, marioTex); // Reusing run sprite for firing
	sprites->Add(MARIO_FIRE_FLAG_PULL_SPRITE_1, 112, 80, 127, 111, marioTex);
	sprites->Add(MARIO_FIRE_FLAG_PULL_SPRITE_2, 128, 80, 143, 111, marioTex);

	int swimIds[4] = { MARIO_FIRE_SWIM_SPRITE_1, MARIO_FIRE_SWIM_SPRITE_2, MARIO_FIRE_SWIM_SPRITE_3, MARIO_FIRE_SWIM_SPRITE_4 };
	LoadHorizontalSheet(swimIds, 4, 160, 80, 16, 32, marioTex);

	// --- Animations ---
	anim = new Animation(300);
	anim->Add(MARIO_FIRE_IDLE_SPRITE_1);
	anims->Add(MARIO_FIRE_IDLE_ANIM_ID, anim);

	anim = new Animation(100);
	anim->Add(MARIO_FIRE_RUN_SPRITE_1);
	anim->Add(MARIO_FIRE_RUN_SPRITE_2);
	anim->Add(MARIO_FIRE_RUN_SPRITE_3);
	anims->Add(MARIO_FIRE_RUN_ANIM_ID, anim);

	anim = new Animation(100);
	anim->Add(MARIO_FIRE_SKID_SPRITE_1);
	anims->Add(MARIO_FIRE_SKID_ANIM_ID, anim);

	anim = new Animation(100);
	anim->Add(MARIO_FIRE_JUMP_SPRITE_1);
	anims->Add(MARIO_FIRE_JUMP_ANIM_ID, anim);

	anim = new Animation(100);
	anim->Add(MARIO_FIRE_DUCK_SPRITE_1);
	anims->Add(MARIO_FIRE_DUCK_ANIM_ID, anim);

	anim = new Animation(300);
	anim->Add(MARIO_FIRE_FIRE_SPRITE_1);
	anims->Add(MARIO_FIRE_FIRE_ANIM_ID, anim);

	anim = new Animation(150);
	anim->Add(MARIO_FIRE_FLAG_PULL_SPRITE_1, 150);
	anim->Add(MARIO_FIRE_FLAG_PULL_SPRITE_2, 150);
	anims->Add(MARIO_FIRE_FLAG_PULL_ANIM_ID, anim);

	anim = new Animation(swimIds, 4, 150);
	anims->Add(MARIO_FIRE_SWIM_ANIM_ID, anim);
}

void Mario::LoadSmallStarman()
{
	auto marioTex = Textures::GetInstance()->Get(MARIO_TEX_ID);
	auto anims = Animations::GetInstance();
	auto sprites = Sprites::GetInstance();
	Animation* anim = nullptr;

	// =========================================================
	// SMALL STARMAN
	// =========================================================

	// --- Idle ---
	sprites->Add(STARMAN_SMALL_IDLE_SPRITE_1, 0, 112, 15, 127, marioTex);
	sprites->Add(STARMAN_SMALL_IDLE_SPRITE_2, 0, 160, 15, 175, marioTex);
	sprites->Add(STARMAN_SMALL_IDLE_SPRITE_3, 0, 208, 15, 223, marioTex);

	anim = new Animation(100);
	anim->Add(STARMAN_SMALL_IDLE_SPRITE_1);
	anim->Add(STARMAN_SMALL_IDLE_SPRITE_2);
	anim->Add(STARMAN_SMALL_IDLE_SPRITE_3);
	anims->Add(STARMAN_SMALL_IDLE_ANIM_ID, anim);

	// --- Walk ---
	sprites->Add(STARMAN_SMALL_RUN_SPRITE_1, 16, 112, 31, 127, marioTex);
	sprites->Add(STARMAN_SMALL_RUN_SPRITE_2, 16, 160, 31, 175, marioTex);
	sprites->Add(STARMAN_SMALL_RUN_SPRITE_3, 16, 208, 31, 223, marioTex);
	sprites->Add(STARMAN_SMALL_RUN_SPRITE_4, 32, 112, 47, 127, marioTex);
	sprites->Add(STARMAN_SMALL_RUN_SPRITE_5, 32, 160, 47, 175, marioTex);
	sprites->Add(STARMAN_SMALL_RUN_SPRITE_6, 32, 208, 47, 223, marioTex);
	sprites->Add(STARMAN_SMALL_RUN_SPRITE_7, 48, 112, 63, 127, marioTex);
	sprites->Add(STARMAN_SMALL_RUN_SPRITE_8, 48, 160, 63, 175, marioTex);
	sprites->Add(STARMAN_SMALL_RUN_SPRITE_9, 48, 208, 63, 223, marioTex);

	anim = new Animation(33);
	anim->Add(STARMAN_SMALL_RUN_SPRITE_1);
	anim->Add(STARMAN_SMALL_RUN_SPRITE_2);
	anim->Add(STARMAN_SMALL_RUN_SPRITE_3);
	anim->Add(STARMAN_SMALL_RUN_SPRITE_4);
	anim->Add(STARMAN_SMALL_RUN_SPRITE_5);
	anim->Add(STARMAN_SMALL_RUN_SPRITE_6);
	anim->Add(STARMAN_SMALL_RUN_SPRITE_7);
	anim->Add(STARMAN_SMALL_RUN_SPRITE_8);
	anim->Add(STARMAN_SMALL_RUN_SPRITE_9);
	anims->Add(STARMAN_SMALL_RUN_ANIM_ID, anim);

	// --- Skid ---
	sprites->Add(STARMAN_SMALL_SKID_SPRITE_1, 64, 112, 79, 127, marioTex);
	sprites->Add(STARMAN_SMALL_SKID_SPRITE_2, 64, 160, 79, 175, marioTex);
	sprites->Add(STARMAN_SMALL_SKID_SPRITE_3, 64, 208, 79, 223, marioTex);

	anim = new Animation(100);
	anim->Add(STARMAN_SMALL_SKID_SPRITE_1);
	anim->Add(STARMAN_SMALL_SKID_SPRITE_2);
	anim->Add(STARMAN_SMALL_SKID_SPRITE_3);
	anims->Add(STARMAN_SMALL_SKID_ANIM_ID, anim);

	// --- Jump ---
	sprites->Add(STARMAN_SMALL_JUMP_SPRITE_1, 80, 112, 95, 127, marioTex);
	sprites->Add(STARMAN_SMALL_JUMP_SPRITE_2, 80, 160, 95, 175, marioTex);
	sprites->Add(STARMAN_SMALL_JUMP_SPRITE_3, 80, 208, 95, 223, marioTex);

	anim = new Animation(100);
	anim->Add(STARMAN_SMALL_JUMP_SPRITE_1);
	anim->Add(STARMAN_SMALL_JUMP_SPRITE_2);
	anim->Add(STARMAN_SMALL_JUMP_SPRITE_3);
	anims->Add(STARMAN_SMALL_JUMP_ANIM_ID, anim);

	// --- Flag Pull ---
	sprites->Add(STARMAN_SMALL_FLAG_PULL_SPRITE_1, 112, 112, 127, 127, marioTex);
	sprites->Add(STARMAN_SMALL_FLAG_PULL_SPRITE_2, 112, 160, 127, 175, marioTex);
	sprites->Add(STARMAN_SMALL_FLAG_PULL_SPRITE_3, 112, 208, 127, 223, marioTex);
	sprites->Add(STARMAN_SMALL_FLAG_PULL_SPRITE_4, 128, 112, 143, 127, marioTex);
	sprites->Add(STARMAN_SMALL_FLAG_PULL_SPRITE_5, 128, 160, 143, 175, marioTex);
	sprites->Add(STARMAN_SMALL_FLAG_PULL_SPRITE_6, 128, 208, 143, 223, marioTex);

	anim = new Animation(33);
	anim->Add(STARMAN_SMALL_FLAG_PULL_SPRITE_1);
	anim->Add(STARMAN_SMALL_FLAG_PULL_SPRITE_2);
	anim->Add(STARMAN_SMALL_FLAG_PULL_SPRITE_3);
	anim->Add(STARMAN_SMALL_FLAG_PULL_SPRITE_4);
	anim->Add(STARMAN_SMALL_FLAG_PULL_SPRITE_5);
	anim->Add(STARMAN_SMALL_FLAG_PULL_SPRITE_6);
	anims->Add(STARMAN_SMALL_FLAG_PULL_ANIM_ID, anim);


	// swim
	int swimIdsBatch1[4] = { 
		STARMAN_SWIM_SPRITE_1, 
		STARMAN_SWIM_SPRITE_2,
		STARMAN_SWIM_SPRITE_3,
		STARMAN_SWIM_SPRITE_4,
	};
	LoadHorizontalSheet(swimIdsBatch1, 4, 160, 112, 16, 16, marioTex);

	int swimIdsBatch2[4] = {
		STARMAN_SWIM_SPRITE_5,
		STARMAN_SWIM_SPRITE_6,
		STARMAN_SWIM_SPRITE_7,
		STARMAN_SWIM_SPRITE_8,
	};
	LoadHorizontalSheet(swimIdsBatch2, 4, 160, 160, 16, 16, marioTex);

	int swimIdsBatch3[4] = {
		STARMAN_SWIM_SPRITE_9,
		STARMAN_SWIM_SPRITE_10,
		STARMAN_SWIM_SPRITE_11,
		STARMAN_SWIM_SPRITE_12,
	};

	LoadHorizontalSheet(swimIdsBatch3, 4, 160, 208, 16, 16, marioTex);
	anim = new Animation(33);
	
	anim->Add(STARMAN_SWIM_SPRITE_1);
	anim->Add(STARMAN_SWIM_SPRITE_5);
	anim->Add(STARMAN_SWIM_SPRITE_9);
	anim->Add(STARMAN_SWIM_SPRITE_2);
	anim->Add(STARMAN_SWIM_SPRITE_6);
	anim->Add(STARMAN_SWIM_SPRITE_10);
	anim->Add(STARMAN_SWIM_SPRITE_3);
	anim->Add(STARMAN_SWIM_SPRITE_7);
	anim->Add(STARMAN_SWIM_SPRITE_11);
	anim->Add(STARMAN_SWIM_SPRITE_4);
	anim->Add(STARMAN_SWIM_SPRITE_8);
	anim->Add(STARMAN_SWIM_SPRITE_12);

	anims->Add(STARMAN_SWIM_ANIM_ID, anim);

}

void Mario::LoadBigStarman()
{
	auto marioTex = Textures::GetInstance()->Get(MARIO_TEX_ID);
	auto anims = Animations::GetInstance();
	auto sprites = Sprites::GetInstance();
	Animation* anim = nullptr;

	// =========================================================
	// BIG STARMAN
	// =========================================================

	// --- Idle ---
	sprites->Add(STARMAN_BIG_IDLE_SPRITE_1, 0, 128, 15, 159, marioTex);
	sprites->Add(STARMAN_BIG_IDLE_SPRITE_2, 0, 176, 15, 207, marioTex);
	sprites->Add(STARMAN_BIG_IDLE_SPRITE_3, 0, 224, 15, 255, marioTex);

	anim = new Animation(100);
	anim->Add(STARMAN_BIG_IDLE_SPRITE_1);
	anim->Add(STARMAN_BIG_IDLE_SPRITE_2);
	anim->Add(STARMAN_BIG_IDLE_SPRITE_3);
	anims->Add(STARMAN_BIG_IDLE_ANIM_ID, anim);

	// --- Walk ---
	sprites->Add(STARMAN_BIG_RUN_SPRITE_1, 16, 128, 31, 159, marioTex);
	sprites->Add(STARMAN_BIG_RUN_SPRITE_2, 16, 176, 31, 207, marioTex);
	sprites->Add(STARMAN_BIG_RUN_SPRITE_3, 16, 224, 31, 255, marioTex);
	sprites->Add(STARMAN_BIG_RUN_SPRITE_4, 32, 128, 47, 159, marioTex);
	sprites->Add(STARMAN_BIG_RUN_SPRITE_5, 32, 176, 47, 207, marioTex);
	sprites->Add(STARMAN_BIG_RUN_SPRITE_6, 32, 224, 47, 255, marioTex);
	sprites->Add(STARMAN_BIG_RUN_SPRITE_7, 48, 128, 63, 159, marioTex);
	sprites->Add(STARMAN_BIG_RUN_SPRITE_8, 48, 176, 63, 207, marioTex);
	sprites->Add(STARMAN_BIG_RUN_SPRITE_9, 48, 224, 63, 255, marioTex);

	anim = new Animation(15);
	anim->Add(STARMAN_BIG_RUN_SPRITE_1);
	anim->Add(STARMAN_BIG_RUN_SPRITE_2);
	anim->Add(STARMAN_BIG_RUN_SPRITE_3);
	anim->Add(STARMAN_BIG_RUN_SPRITE_4);
	anim->Add(STARMAN_BIG_RUN_SPRITE_5);
	anim->Add(STARMAN_BIG_RUN_SPRITE_6);
	anim->Add(STARMAN_BIG_RUN_SPRITE_7);
	anim->Add(STARMAN_BIG_RUN_SPRITE_8);
	anim->Add(STARMAN_BIG_RUN_SPRITE_9);
	anims->Add(STARMAN_BIG_RUN_ANIM_ID, anim);

	// --- Skid ---
	sprites->Add(STARMAN_BIG_SKID_SPRITE_1, 64, 128, 79, 159, marioTex);
	sprites->Add(STARMAN_BIG_SKID_SPRITE_2, 64, 176, 79, 207, marioTex);
	sprites->Add(STARMAN_BIG_SKID_SPRITE_3, 64, 224, 79, 255, marioTex);

	anim = new Animation(100);
	anim->Add(STARMAN_BIG_SKID_SPRITE_1);
	anim->Add(STARMAN_BIG_SKID_SPRITE_2);
	anim->Add(STARMAN_BIG_SKID_SPRITE_3);
	anims->Add(STARMAN_BIG_SKID_ANIM_ID, anim);

	// --- Jump ---
	sprites->Add(STARMAN_BIG_JUMP_SPRITE_1, 80, 128, 95, 159, marioTex);
	sprites->Add(STARMAN_BIG_JUMP_SPRITE_2, 80, 176, 95, 207, marioTex);
	sprites->Add(STARMAN_BIG_JUMP_SPRITE_3, 80, 224, 95, 255, marioTex);

	anim = new Animation(100);
	anim->Add(STARMAN_BIG_JUMP_SPRITE_1);
	anim->Add(STARMAN_BIG_JUMP_SPRITE_2);
	anim->Add(STARMAN_BIG_JUMP_SPRITE_3);
	anims->Add(STARMAN_BIG_JUMP_ANIM_ID, anim);

	// --- Duck ---
	sprites->Add(STARMAN_BIG_DUCK_SPRITE_1, 96, 128, 111, 159, marioTex);
	sprites->Add(STARMAN_BIG_DUCK_SPRITE_2, 96, 176, 111, 207, marioTex);
	sprites->Add(STARMAN_BIG_DUCK_SPRITE_3, 96, 224, 111, 255, marioTex);

	anim = new Animation(100);
	anim->Add(STARMAN_BIG_DUCK_SPRITE_1);
	anim->Add(STARMAN_BIG_DUCK_SPRITE_2);
	anim->Add(STARMAN_BIG_DUCK_SPRITE_3);
	anims->Add(STARMAN_BIG_DUCK_ANIM_ID, anim);

	// --- Flag Pull ---
	sprites->Add(STARMAN_BIG_FLAG_PULL_SPRITE_1, 112, 128, 127, 159, marioTex);
	sprites->Add(STARMAN_BIG_FLAG_PULL_SPRITE_2, 112, 176, 127, 207, marioTex);
	sprites->Add(STARMAN_BIG_FLAG_PULL_SPRITE_3, 112, 224, 127, 255, marioTex);
	sprites->Add(STARMAN_BIG_FLAG_PULL_SPRITE_4, 128, 128, 143, 159, marioTex);
	sprites->Add(STARMAN_BIG_FLAG_PULL_SPRITE_5, 128, 176, 143, 207, marioTex);
	sprites->Add(STARMAN_BIG_FLAG_PULL_SPRITE_6, 128, 224, 143, 255, marioTex);

	anim = new Animation(33);
	anim->Add(STARMAN_BIG_FLAG_PULL_SPRITE_1);
	anim->Add(STARMAN_BIG_FLAG_PULL_SPRITE_2);
	anim->Add(STARMAN_BIG_FLAG_PULL_SPRITE_3);
	anim->Add(STARMAN_BIG_FLAG_PULL_SPRITE_4);
	anim->Add(STARMAN_BIG_FLAG_PULL_SPRITE_5);
	anim->Add(STARMAN_BIG_FLAG_PULL_SPRITE_6);
	anims->Add(STARMAN_BIG_FLAG_PULL_ANIM_ID, anim);


	// swim
	int swimIdsBatch1[4] = {
		STARMAN_BIG_SWIM_SPRITE_1,
		STARMAN_BIG_SWIM_SPRITE_2,
		STARMAN_BIG_SWIM_SPRITE_3,
		STARMAN_BIG_SWIM_SPRITE_4,
	};
	LoadHorizontalSheet(swimIdsBatch1, 4, 160, 128, 16, 32, marioTex);

	int swimIdsBatch2[4] = {
		STARMAN_BIG_SWIM_SPRITE_5,
		STARMAN_BIG_SWIM_SPRITE_6,
		STARMAN_BIG_SWIM_SPRITE_7,
		STARMAN_BIG_SWIM_SPRITE_8,
	};
	LoadHorizontalSheet(swimIdsBatch2, 4, 160, 176, 16, 32, marioTex);

	int swimIdsBatch3[4] = {
		STARMAN_BIG_SWIM_SPRITE_9,
		STARMAN_BIG_SWIM_SPRITE_10,
		STARMAN_BIG_SWIM_SPRITE_11,
		STARMAN_BIG_SWIM_SPRITE_12,
	};

	LoadHorizontalSheet(swimIdsBatch3, 4, 160, 224, 16, 32, marioTex);
	anim = new Animation(33);

	anim->Add(STARMAN_BIG_SWIM_SPRITE_1);
	anim->Add(STARMAN_BIG_SWIM_SPRITE_5);
	anim->Add(STARMAN_BIG_SWIM_SPRITE_9);
	anim->Add(STARMAN_BIG_SWIM_SPRITE_2);
	anim->Add(STARMAN_BIG_SWIM_SPRITE_6);
	anim->Add(STARMAN_BIG_SWIM_SPRITE_10);
	anim->Add(STARMAN_BIG_SWIM_SPRITE_3);
	anim->Add(STARMAN_BIG_SWIM_SPRITE_7);
	anim->Add(STARMAN_BIG_SWIM_SPRITE_11);
	anim->Add(STARMAN_BIG_SWIM_SPRITE_4);
	anim->Add(STARMAN_BIG_SWIM_SPRITE_8);
	anim->Add(STARMAN_BIG_SWIM_SPRITE_12);

	anims->Add(STARMAN_BIG_SWIM_ANIM_ID, anim);
}

void Mario::LoadSpriteAndAnimation()
{

	LoadSmallNormalMario();
	LoadBigNormalMario();
	LoadFireMario();
	LoadSmallStarman();
	LoadBigStarman();
}


