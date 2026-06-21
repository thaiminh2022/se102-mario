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
	sprites->Add(MARIO_WALK_SPRITE_1, 16, 0, 31, 15, marioTex);
	sprites->Add(MARIO_WALK_SPRITE_2, 32, 0, 47, 15, marioTex);
	sprites->Add(MARIO_WALK_SPRITE_3, 48, 0, 63, 15, marioTex);
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

	// IDLE
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(MARIO_IDLE_SPRITE_1);
	anims->Add(MARIO_IDLE_ANIM_ID, anim);

	// WALKING
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(MARIO_WALK_SPRITE_1);
	anim->Add(MARIO_WALK_SPRITE_2);
	anim->Add(MARIO_WALK_SPRITE_3);
	anims->Add(MARIO_WALK_ANIM_ID, anim);

	// SKIDDING
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(MARIO_SKID_SPRITE_1);
	anims->Add(MARIO_SKID_ANIM_ID, anim);

	//JUMPING
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(MARIO_JUMP_SPRITE_1);
	anims->Add(MARIO_JUMP_ANIM_ID, anim);

	// DEATH
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(MARIO_DEATH_SPRITE_1);
	anims->Add(MARIO_DEATH_ANIM_ID, anim);

	// GROWING
	anim = new Animation(ANIMATION_FAST_FRAMETIME);
	anim->Add(MARIO_GROWBIG_SPRITE_1);
	anim->Add(MARIO_GROWBIG_SPRITE_2);
	anim->Add(MARIO_GROWBIG_SPRITE_1);
	anim->Add(MARIO_GROWBIG_SPRITE_2);
	anim->Add(MARIO_GROWBIG_SPRITE_3);
	anim->Add(MARIO_GROWBIG_SPRITE_2);
	anim->Add(MARIO_GROWBIG_SPRITE_3);
	anims->Add(MARIO_GROWBIG_ANIM_ID, anim);

	// FLAG PULLING
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(MARIO_FLAG_PULL_SPRITE_1);
	anim->Add(MARIO_FLAG_PULL_SPRITE_2);
	anims->Add(MARIO_FLAG_PULL_ANIM_ID, anim);

	// SWIMMING
	anim = new Animation(swimIds, 4, ANIMATION_DEFAULT_FRAMETIME);
	anims->Add(MARIO_SWIM_ANIM_ID, anim);
	
	// RUNNING
	anim = new Animation(ANIMATION_FAST_FRAMETIME);
	anim->Add(MARIO_WALK_SPRITE_1);
	anim->Add(MARIO_WALK_SPRITE_2);
	anim->Add(MARIO_WALK_SPRITE_3);
	anims->Add(MARIO_RUN_ANIM_ID, anim);
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
	sprites->Add(MARIO_BIG_WALK_SPRITE_1, 16, 16, 31, 47, marioTex);
	sprites->Add(MARIO_BIG_WALK_SPRITE_2, 32, 16, 47, 47, marioTex);
	sprites->Add(MARIO_BIG_WALK_SPRITE_3, 48, 16, 63, 47, marioTex);
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

	// IDLE
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(MARIO_BIG_IDLE_SPRITE_1);
	anims->Add(MARIO_BIG_IDLE_ANIM_ID, anim);

	// WALKING
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(MARIO_BIG_WALK_SPRITE_1);
	anim->Add(MARIO_BIG_WALK_SPRITE_2);
	anim->Add(MARIO_BIG_WALK_SPRITE_3);
	anims->Add(MARIO_BIG_WALK_ANIM_ID, anim);

	// SKIDDING
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(MARIO_BIG_SKID_SPRITE_1);
	anims->Add(MARIO_BIG_SKID_ANIM_ID, anim);

	//JUMPING
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(MARIO_BIG_JUMP_SPRITE_1);
	anims->Add(MARIO_BIG_JUMP_ANIM_ID, anim);

	// DUCKING
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(MARIO_BIG_DUCK_SPRITE_1);
	anims->Add(MARIO_BIG_DUCK_ANIM_ID, anim);

	// SHRINKING
	anim = new Animation(ANIMATION_FAST_FRAMETIME);
	anim->Add(MARIO_SHRINK_SPRITE_1);
	anim->Add(MARIO_SHRINK_SPRITE_3);
	anim->Add(MARIO_SHRINK_SPRITE_1);
	anim->Add(MARIO_SHRINK_SPRITE_3);
	anim->Add(MARIO_SHRINK_SPRITE_1);
	anim->Add(MARIO_SHRINK_SPRITE_3);
	anim->Add(MARIO_SHRINK_SPRITE_1);
	anim->Add(MARIO_SHRINK_SPRITE_3);
	anim->Add(MARIO_SHRINK_SPRITE_2);
	anim->Add(MARIO_SHRINK_SPRITE_3);
	anim->Add(MARIO_SHRINK_SPRITE_2);
	anim->Add(MARIO_SHRINK_SPRITE_3);
	anim->Add(MARIO_SHRINK_SPRITE_2);
	anim->Add(MARIO_SHRINK_SPRITE_3);
	anim->Add(MARIO_SHRINK_SPRITE_2);
	anims->Add(MARIO_SHRINK_ANIM_ID, anim);

	// FLAG PULLING
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(MARIO_BIG_FLAG_PULL_SPRITE_1);
	anim->Add(MARIO_BIG_FLAG_PULL_SPRITE_2);
	anims->Add(MARIO_BIG_FLAG_PULL_ANIM_ID, anim);

	// SWIMMING
	anim = new Animation(swimIds, 4, ANIMATION_DEFAULT_FRAMETIME);
	anims->Add(MARIO_BIG_SWIM_ANIM_ID, anim);

	// RUNNING
	anim = new Animation(ANIMATION_FAST_FRAMETIME);
	anim->Add(MARIO_BIG_WALK_SPRITE_1);
	anim->Add(MARIO_BIG_WALK_SPRITE_2);
	anim->Add(MARIO_BIG_WALK_SPRITE_3);
	anims->Add(MARIO_BIG_RUN_ANIM_ID, anim);
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
	sprites->Add(MARIO_FIRE_WALK_SPRITE_1, 16, 80, 31, 111, marioTex);
	sprites->Add(MARIO_FIRE_WALK_SPRITE_2, 32, 80, 47, 111, marioTex);
	sprites->Add(MARIO_FIRE_WALK_SPRITE_3, 48, 80, 63, 111, marioTex);
	sprites->Add(MARIO_FIRE_SKID_SPRITE_1, 64, 80, 79, 111, marioTex);
	sprites->Add(MARIO_FIRE_JUMP_SPRITE_1, 80, 80, 95, 111, marioTex);
	sprites->Add(MARIO_FIRE_DUCK_SPRITE_1, 96, 80, 111, 111, marioTex);
	sprites->Add(MARIO_FIRE_FIRE_SPRITE_1, 16, 80, 31, 111, marioTex); // Reusing run sprite for firing
	sprites->Add(MARIO_FIRE_FLAG_PULL_SPRITE_1, 112, 80, 127, 111, marioTex);
	sprites->Add(MARIO_FIRE_FLAG_PULL_SPRITE_2, 128, 80, 143, 111, marioTex);

	int swimIds[4] = { MARIO_FIRE_SWIM_SPRITE_1, MARIO_FIRE_SWIM_SPRITE_2, MARIO_FIRE_SWIM_SPRITE_3, MARIO_FIRE_SWIM_SPRITE_4 };
	LoadHorizontalSheet(swimIds, 4, 160, 80, 16, 32, marioTex);

	// --- Animations ---

	// IDLE
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(MARIO_FIRE_IDLE_SPRITE_1);
	anims->Add(MARIO_FIRE_IDLE_ANIM_ID, anim);

	// WALKING
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(MARIO_FIRE_WALK_SPRITE_1);
	anim->Add(MARIO_FIRE_WALK_SPRITE_2);
	anim->Add(MARIO_FIRE_WALK_SPRITE_3);
	anims->Add(MARIO_FIRE_WALK_ANIM_ID, anim);

	// SKIDDING
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(MARIO_FIRE_SKID_SPRITE_1);
	anims->Add(MARIO_FIRE_SKID_ANIM_ID, anim);

	//JUMPING
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(MARIO_FIRE_JUMP_SPRITE_1);
	anims->Add(MARIO_FIRE_JUMP_ANIM_ID, anim);

	// DUCKING
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(MARIO_FIRE_DUCK_SPRITE_1);
	anims->Add(MARIO_FIRE_DUCK_ANIM_ID, anim);

	// FIRING
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(MARIO_FIRE_FIRE_SPRITE_1);
	anims->Add(MARIO_FIRE_FIRE_ANIM_ID, anim);

	// FLAG PULLING
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(MARIO_FIRE_FLAG_PULL_SPRITE_1);
	anim->Add(MARIO_FIRE_FLAG_PULL_SPRITE_2);
	anims->Add(MARIO_FIRE_FLAG_PULL_ANIM_ID, anim);

	// SWIMMING
	anim = new Animation(swimIds, 4, ANIMATION_DEFAULT_FRAMETIME);
	anims->Add(MARIO_FIRE_SWIM_ANIM_ID, anim);

	// RUNNING
	anim = new Animation(ANIMATION_FAST_FRAMETIME);
	anim->Add(MARIO_FIRE_WALK_SPRITE_1);
	anim->Add(MARIO_FIRE_WALK_SPRITE_2);
	anim->Add(MARIO_FIRE_WALK_SPRITE_3);
	anims->Add(MARIO_FIRE_RUN_ANIM_ID, anim);
}

void Mario::LoadStarmanPalette(int idx)
{
	if (idx < 0 || idx > 2) idx = 0;

	auto anims = Animations::GetInstance();

	// =========================================================
	// SMALL STARMAN
	// =========================================================
	anims->Replace(STARMAN_SMALL1_IDLE_ANIM_ID, starmanBlueprints[idx][STARMAN_SMALL1_IDLE_ANIM_ID]->Clone());
	anims->Replace(STARMAN_SMALL1_WALK_ANIM_ID, starmanBlueprints[idx][STARMAN_SMALL1_WALK_ANIM_ID]->Clone());
	anims->Replace(STARMAN_SMALL1_SKID_ANIM_ID, starmanBlueprints[idx][STARMAN_SMALL1_SKID_ANIM_ID]->Clone());
	anims->Replace(STARMAN_SMALL1_JUMP_ANIM_ID, starmanBlueprints[idx][STARMAN_SMALL1_JUMP_ANIM_ID]->Clone());
	anims->Replace(STARMAN_SMALL1_FLAG_PULL_ANIM_ID, starmanBlueprints[idx][STARMAN_SMALL1_FLAG_PULL_ANIM_ID]->Clone());
	anims->Replace(STARMAN_SMALL1_SWIM_ANIM_ID, starmanBlueprints[idx][STARMAN_SMALL1_SWIM_ANIM_ID]->Clone());
	anims->Replace(STARMAN_SMALL1_RUN_ANIM_ID, starmanBlueprints[idx][STARMAN_SMALL1_RUN_ANIM_ID]->Clone());

	// =========================================================
	// BIG STARMAN
	// =========================================================
	anims->Replace(STARMAN_BIG1_IDLE_ANIM_ID, starmanBlueprints[idx][STARMAN_BIG1_IDLE_ANIM_ID]->Clone());
	anims->Replace(STARMAN_BIG1_WALK_ANIM_ID, starmanBlueprints[idx][STARMAN_BIG1_WALK_ANIM_ID]->Clone());
	anims->Replace(STARMAN_BIG1_SKID_ANIM_ID, starmanBlueprints[idx][STARMAN_BIG1_SKID_ANIM_ID]->Clone());
	anims->Replace(STARMAN_BIG1_JUMP_ANIM_ID, starmanBlueprints[idx][STARMAN_BIG1_JUMP_ANIM_ID]->Clone());
	anims->Replace(STARMAN_BIG1_DUCK_ANIM_ID, starmanBlueprints[idx][STARMAN_BIG1_DUCK_ANIM_ID]->Clone());
	anims->Replace(STARMAN_BIG1_FLAG_PULL_ANIM_ID, starmanBlueprints[idx][STARMAN_BIG1_FLAG_PULL_ANIM_ID]->Clone());
	anims->Replace(STARMAN_BIG1_SWIM_ANIM_ID, starmanBlueprints[idx][STARMAN_BIG1_SWIM_ANIM_ID]->Clone());
	anims->Replace(STARMAN_BIG1_RUN_ANIM_ID, starmanBlueprints[idx][STARMAN_BIG1_RUN_ANIM_ID]->Clone());
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
	sprites->Add(STARMAN_SMALL1_IDLE_SPRITE_1, 0, 112, 15, 127, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_SMALL1_IDLE_SPRITE_1);
	starmanBlueprints[0][STARMAN_SMALL1_IDLE_ANIM_ID] = anim;

	sprites->Add(STARMAN_SMALL2_IDLE_SPRITE_1, 0, 160, 15, 175, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_SMALL2_IDLE_SPRITE_1);
	starmanBlueprints[1][STARMAN_SMALL1_IDLE_ANIM_ID] = anim;

	sprites->Add(STARMAN_SMALL3_IDLE_SPRITE_1, 0, 208, 15, 223, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_SMALL3_IDLE_SPRITE_1);
	starmanBlueprints[2][STARMAN_SMALL1_IDLE_ANIM_ID] = anim;

	// --- Walk ---
	sprites->Add(STARMAN_SMALL1_WALK_SPRITE_1, 16, 112, 31, 127, marioTex);
	sprites->Add(STARMAN_SMALL1_WALK_SPRITE_2, 32, 112, 47, 127, marioTex);
	sprites->Add(STARMAN_SMALL1_WALK_SPRITE_3, 48, 112, 63, 127, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_SMALL1_WALK_SPRITE_1);
	anim->Add(STARMAN_SMALL1_WALK_SPRITE_2);
	anim->Add(STARMAN_SMALL1_WALK_SPRITE_3);
	starmanBlueprints[0][STARMAN_SMALL1_WALK_ANIM_ID] = anim;


	sprites->Add(STARMAN_SMALL2_WALK_SPRITE_1, 16, 160, 31, 175, marioTex);
	sprites->Add(STARMAN_SMALL2_WALK_SPRITE_2, 32, 160, 47, 175, marioTex);
	sprites->Add(STARMAN_SMALL2_WALK_SPRITE_3, 48, 160, 63, 175, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_SMALL2_WALK_SPRITE_1);
	anim->Add(STARMAN_SMALL2_WALK_SPRITE_2);
	anim->Add(STARMAN_SMALL2_WALK_SPRITE_3);
	starmanBlueprints[1][STARMAN_SMALL1_WALK_ANIM_ID] = anim;

	sprites->Add(STARMAN_SMALL3_WALK_SPRITE_1, 16, 208, 31, 223, marioTex);
	sprites->Add(STARMAN_SMALL3_WALK_SPRITE_2, 32, 208, 47, 223, marioTex);
	sprites->Add(STARMAN_SMALL3_WALK_SPRITE_3, 48, 208, 63, 223, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_SMALL3_WALK_SPRITE_1);
	anim->Add(STARMAN_SMALL3_WALK_SPRITE_2);
	anim->Add(STARMAN_SMALL3_WALK_SPRITE_3);
	starmanBlueprints[2][STARMAN_SMALL1_WALK_ANIM_ID] = anim;

	// --- Skid ---
	sprites->Add(STARMAN_SMALL1_SKID_SPRITE_1, 64, 112, 79, 127, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_SMALL1_SKID_SPRITE_1);
	starmanBlueprints[0][STARMAN_SMALL1_SKID_ANIM_ID] = anim;

	sprites->Add(STARMAN_SMALL2_SKID_SPRITE_1, 64, 160, 79, 175, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_SMALL2_SKID_SPRITE_1);
	starmanBlueprints[1][STARMAN_SMALL1_SKID_ANIM_ID] = anim;

	sprites->Add(STARMAN_SMALL3_SKID_SPRITE_1, 64, 208, 79, 223, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_SMALL3_SKID_SPRITE_1);
	starmanBlueprints[2][STARMAN_SMALL1_SKID_ANIM_ID] = anim;

	// --- Jump ---
	sprites->Add(STARMAN_SMALL1_JUMP_SPRITE_1, 80, 112, 95, 127, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_SMALL1_JUMP_SPRITE_1);
	starmanBlueprints[0][STARMAN_SMALL1_JUMP_ANIM_ID] = anim;

	sprites->Add(STARMAN_SMALL2_JUMP_SPRITE_1, 80, 160, 95, 175, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_SMALL2_JUMP_SPRITE_1);
	starmanBlueprints[1][STARMAN_SMALL1_JUMP_ANIM_ID] = anim;

	sprites->Add(STARMAN_SMALL3_JUMP_SPRITE_1, 80, 208, 95, 223, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_SMALL3_JUMP_SPRITE_1);
	starmanBlueprints[2][STARMAN_SMALL1_JUMP_ANIM_ID] = anim;

	// --- Flag Pull ---
	sprites->Add(STARMAN_SMALL1_FLAG_PULL_SPRITE_1, 112, 112, 127, 127, marioTex);
	sprites->Add(STARMAN_SMALL1_FLAG_PULL_SPRITE_2, 112, 160, 127, 175, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_SMALL1_FLAG_PULL_SPRITE_1);
	anim->Add(STARMAN_SMALL1_FLAG_PULL_SPRITE_2);
	starmanBlueprints[0][STARMAN_SMALL1_FLAG_PULL_ANIM_ID] = anim;

	sprites->Add(STARMAN_SMALL2_FLAG_PULL_SPRITE_1, 112, 208, 127, 223, marioTex);
	sprites->Add(STARMAN_SMALL2_FLAG_PULL_SPRITE_2, 128, 112, 143, 127, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_SMALL2_FLAG_PULL_SPRITE_1);
	anim->Add(STARMAN_SMALL2_FLAG_PULL_SPRITE_2);
	starmanBlueprints[1][STARMAN_SMALL1_FLAG_PULL_ANIM_ID] = anim;


	sprites->Add(STARMAN_SMALL3_FLAG_PULL_SPRITE_1, 128, 160, 143, 175, marioTex);
	sprites->Add(STARMAN_SMALL3_FLAG_PULL_SPRITE_2, 128, 208, 143, 223, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_SMALL3_FLAG_PULL_SPRITE_1);
	anim->Add(STARMAN_SMALL3_FLAG_PULL_SPRITE_2);
	starmanBlueprints[2][STARMAN_SMALL1_FLAG_PULL_ANIM_ID] = anim;


	// swim
	int swimIdsBatch1[4] = {
		STARMAN_SMALL1_SWIM_SPRITE_1,
		STARMAN_SMALL1_SWIM_SPRITE_2,
		STARMAN_SMALL1_SWIM_SPRITE_3,
		STARMAN_SMALL1_SWIM_SPRITE_4,
	};
	LoadHorizontalSheet(swimIdsBatch1, 4, 160, 112, 16, 16, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_SMALL1_SWIM_SPRITE_1);
	anim->Add(STARMAN_SMALL1_SWIM_SPRITE_2);
	anim->Add(STARMAN_SMALL1_SWIM_SPRITE_3);
	anim->Add(STARMAN_SMALL1_SWIM_SPRITE_4);
	starmanBlueprints[0][STARMAN_SMALL1_SWIM_ANIM_ID] = anim;


	int swimIdsBatch2[4] = {
		STARMAN_SMALL2_SWIM_SPRITE_1,
		STARMAN_SMALL2_SWIM_SPRITE_2,
		STARMAN_SMALL2_SWIM_SPRITE_3,
		STARMAN_SMALL2_SWIM_SPRITE_4,
	};
	LoadHorizontalSheet(swimIdsBatch2, 4, 160, 160, 16, 16, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_SMALL2_SWIM_SPRITE_1);
	anim->Add(STARMAN_SMALL2_SWIM_SPRITE_2);
	anim->Add(STARMAN_SMALL2_SWIM_SPRITE_3);
	anim->Add(STARMAN_SMALL2_SWIM_SPRITE_4);
	starmanBlueprints[1][STARMAN_SMALL1_SWIM_ANIM_ID] = anim;

	int swimIdsBatch3[4] = {
		STARMAN_SMALL3_SWIM_SPRITE_1,
		STARMAN_SMALL3_SWIM_SPRITE_2,
		STARMAN_SMALL3_SWIM_SPRITE_3,
		STARMAN_SMALL3_SWIM_SPRITE_4,
	};
	LoadHorizontalSheet(swimIdsBatch3, 4, 160, 208, 16, 16, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_SMALL3_SWIM_SPRITE_1);
	anim->Add(STARMAN_SMALL3_SWIM_SPRITE_2);
	anim->Add(STARMAN_SMALL3_SWIM_SPRITE_3);
	anim->Add(STARMAN_SMALL3_SWIM_SPRITE_4);
	starmanBlueprints[2][STARMAN_SMALL1_SWIM_ANIM_ID] = anim;

	anim = new Animation(ANIMATION_FAST_FRAMETIME);
	anim->Add(STARMAN_SMALL1_WALK_SPRITE_1);
	anim->Add(STARMAN_SMALL1_WALK_SPRITE_2);
	anim->Add(STARMAN_SMALL1_WALK_SPRITE_3);
	starmanBlueprints[0][STARMAN_SMALL1_RUN_ANIM_ID] = anim;

	anim = new Animation(ANIMATION_FAST_FRAMETIME);
	anim->Add(STARMAN_SMALL2_WALK_SPRITE_1);
	anim->Add(STARMAN_SMALL2_WALK_SPRITE_2);
	anim->Add(STARMAN_SMALL2_WALK_SPRITE_3);
	starmanBlueprints[1][STARMAN_SMALL1_RUN_ANIM_ID] = anim;

	anim = new Animation(ANIMATION_FAST_FRAMETIME);
	anim->Add(STARMAN_SMALL3_WALK_SPRITE_1);
	anim->Add(STARMAN_SMALL3_WALK_SPRITE_2);
	anim->Add(STARMAN_SMALL3_WALK_SPRITE_3);
	starmanBlueprints[2][STARMAN_SMALL1_RUN_ANIM_ID] = anim;
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
	sprites->Add(STARMAN_BIG1_IDLE_SPRITE_1, 0, 128, 15, 159, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG1_IDLE_SPRITE_1);
	starmanBlueprints[0][STARMAN_BIG1_IDLE_ANIM_ID] = anim;

	sprites->Add(STARMAN_BIG2_IDLE_SPRITE_1, 0, 176, 15, 207, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG2_IDLE_SPRITE_1);
	starmanBlueprints[1][STARMAN_BIG1_IDLE_ANIM_ID] = anim;

	sprites->Add(STARMAN_BIG3_IDLE_SPRITE_1, 0, 224, 15, 255, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG3_IDLE_SPRITE_1);
	starmanBlueprints[2][STARMAN_BIG1_IDLE_ANIM_ID] = anim;

	// --- Walk ---
	sprites->Add(STARMAN_BIG1_WALK_SPRITE_1, 16, 128, 31, 159, marioTex);
	sprites->Add(STARMAN_BIG1_WALK_SPRITE_2, 32, 128, 47, 159, marioTex);
	sprites->Add(STARMAN_BIG1_WALK_SPRITE_3, 48, 128, 63, 159, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG1_WALK_SPRITE_1);
	anim->Add(STARMAN_BIG1_WALK_SPRITE_2);
	anim->Add(STARMAN_BIG1_WALK_SPRITE_3);
	starmanBlueprints[0][STARMAN_BIG1_WALK_ANIM_ID] = anim;


	sprites->Add(STARMAN_BIG2_WALK_SPRITE_1, 16, 176, 31, 207, marioTex);
	sprites->Add(STARMAN_BIG2_WALK_SPRITE_2, 32, 176, 47, 207, marioTex);
	sprites->Add(STARMAN_BIG2_WALK_SPRITE_3, 48, 176, 63, 207, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG2_WALK_SPRITE_1);
	anim->Add(STARMAN_BIG2_WALK_SPRITE_2);
	anim->Add(STARMAN_BIG2_WALK_SPRITE_3);
	starmanBlueprints[1][STARMAN_BIG1_WALK_ANIM_ID] = anim;

	sprites->Add(STARMAN_BIG3_WALK_SPRITE_1, 16, 224, 31, 255, marioTex);
	sprites->Add(STARMAN_BIG3_WALK_SPRITE_2, 32, 224, 47, 255, marioTex);
	sprites->Add(STARMAN_BIG3_WALK_SPRITE_3, 48, 224, 63, 255, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG3_WALK_SPRITE_1);
	anim->Add(STARMAN_BIG3_WALK_SPRITE_2);
	anim->Add(STARMAN_BIG3_WALK_SPRITE_3);
	starmanBlueprints[2][STARMAN_BIG1_WALK_ANIM_ID] = anim;

	// --- Skid ---
	sprites->Add(STARMAN_BIG1_SKID_SPRITE_1, 64, 128, 79, 159, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG1_SKID_SPRITE_1);
	starmanBlueprints[0][STARMAN_BIG1_SKID_ANIM_ID] = anim;

	sprites->Add(STARMAN_BIG2_SKID_SPRITE_1, 64, 176, 79, 207, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG2_SKID_SPRITE_1);
	starmanBlueprints[1][STARMAN_BIG1_SKID_ANIM_ID] = anim;

	sprites->Add(STARMAN_BIG3_SKID_SPRITE_1, 64, 224, 79, 255, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG3_SKID_SPRITE_1);
	starmanBlueprints[2][STARMAN_BIG1_SKID_ANIM_ID] = anim;

	// --- Jump ---
	sprites->Add(STARMAN_BIG1_JUMP_SPRITE_1, 80, 128, 95, 159, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG1_JUMP_SPRITE_1);
	starmanBlueprints[0][STARMAN_BIG1_JUMP_ANIM_ID] = anim;

	sprites->Add(STARMAN_BIG2_JUMP_SPRITE_1, 80, 176, 95, 207, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG2_JUMP_SPRITE_1);
	starmanBlueprints[1][STARMAN_BIG1_JUMP_ANIM_ID] = anim;

	sprites->Add(STARMAN_BIG3_JUMP_SPRITE_1, 80, 224, 95, 255, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG3_JUMP_SPRITE_1);
	starmanBlueprints[2][STARMAN_BIG1_JUMP_ANIM_ID] = anim;


	// --- Duck ---

	sprites->Add(STARMAN_BIG1_DUCK_SPRITE_1, 96, 128, 111, 159, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG1_DUCK_SPRITE_1);
	starmanBlueprints[0][STARMAN_BIG1_DUCK_ANIM_ID] = anim;

	sprites->Add(STARMAN_BIG2_DUCK_SPRITE_1, 96, 176, 111, 207, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG2_DUCK_SPRITE_1);
	starmanBlueprints[1][STARMAN_BIG1_DUCK_ANIM_ID] = anim;

	sprites->Add(STARMAN_BIG3_DUCK_SPRITE_1, 96, 224, 111, 255, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG3_DUCK_SPRITE_1);
	starmanBlueprints[2][STARMAN_BIG1_DUCK_ANIM_ID] = anim;

	// --- Flag Pull ---
	sprites->Add(STARMAN_BIG1_FLAG_PULL_SPRITE_1, 112, 128, 127, 159, marioTex);
	sprites->Add(STARMAN_BIG1_FLAG_PULL_SPRITE_2, 128, 128, 143, 159, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG1_FLAG_PULL_SPRITE_1);
	anim->Add(STARMAN_BIG1_FLAG_PULL_SPRITE_2);
	starmanBlueprints[0][STARMAN_BIG1_FLAG_PULL_ANIM_ID] = anim;

	sprites->Add(STARMAN_BIG2_FLAG_PULL_SPRITE_1, 112, 176, 127, 207, marioTex);
	sprites->Add(STARMAN_BIG2_FLAG_PULL_SPRITE_2, 128, 176, 143, 207, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG2_FLAG_PULL_SPRITE_1);
	anim->Add(STARMAN_BIG2_FLAG_PULL_SPRITE_2);
	starmanBlueprints[1][STARMAN_BIG1_FLAG_PULL_ANIM_ID] = anim;


	sprites->Add(STARMAN_BIG3_FLAG_PULL_SPRITE_1, 112, 224, 127, 255, marioTex);
	sprites->Add(STARMAN_BIG3_FLAG_PULL_SPRITE_2, 128, 224, 143, 255, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG3_FLAG_PULL_SPRITE_1);
	anim->Add(STARMAN_BIG3_FLAG_PULL_SPRITE_2);
	starmanBlueprints[2][STARMAN_BIG1_FLAG_PULL_ANIM_ID] = anim;


	// swim
	int swimIdsBatch1[4] = {
		STARMAN_BIG1_SWIM_SPRITE_1,
		STARMAN_BIG1_SWIM_SPRITE_2,
		STARMAN_BIG1_SWIM_SPRITE_3,
		STARMAN_BIG1_SWIM_SPRITE_4,
	};
	LoadHorizontalSheet(swimIdsBatch1, 4, 160, 128, 16, 32, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG1_SWIM_SPRITE_1);
	anim->Add(STARMAN_BIG1_SWIM_SPRITE_2);
	anim->Add(STARMAN_BIG1_SWIM_SPRITE_3);
	anim->Add(STARMAN_BIG1_SWIM_SPRITE_4);
	starmanBlueprints[0][STARMAN_BIG1_SWIM_ANIM_ID] = anim;


	int swimIdsBatch2[4] = {
		STARMAN_BIG2_SWIM_SPRITE_1,
		STARMAN_BIG2_SWIM_SPRITE_2,
		STARMAN_BIG2_SWIM_SPRITE_3,
		STARMAN_BIG2_SWIM_SPRITE_4,
	};
	LoadHorizontalSheet(swimIdsBatch2, 4, 160, 176, 16, 32, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG2_SWIM_SPRITE_1);
	anim->Add(STARMAN_BIG2_SWIM_SPRITE_2);
	anim->Add(STARMAN_BIG2_SWIM_SPRITE_3);
	anim->Add(STARMAN_BIG2_SWIM_SPRITE_4);
	starmanBlueprints[1][STARMAN_BIG1_SWIM_ANIM_ID] = anim;

	int swimIdsBatch3[4] = {
		STARMAN_BIG3_SWIM_SPRITE_1,
		STARMAN_BIG3_SWIM_SPRITE_2,
		STARMAN_BIG3_SWIM_SPRITE_3,
		STARMAN_BIG3_SWIM_SPRITE_4,
	};
	LoadHorizontalSheet(swimIdsBatch3, 4, 160, 224, 16, 32, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(STARMAN_BIG3_SWIM_SPRITE_1);
	anim->Add(STARMAN_BIG3_SWIM_SPRITE_2);
	anim->Add(STARMAN_BIG3_SWIM_SPRITE_3);
	anim->Add(STARMAN_BIG3_SWIM_SPRITE_4);
	starmanBlueprints[2][STARMAN_BIG1_SWIM_ANIM_ID] = anim;

	anim = new Animation(ANIMATION_FAST_FRAMETIME);
	anim->Add(STARMAN_BIG1_WALK_SPRITE_1);
	anim->Add(STARMAN_BIG1_WALK_SPRITE_2);
	anim->Add(STARMAN_BIG1_WALK_SPRITE_3);
	starmanBlueprints[0][STARMAN_BIG1_RUN_ANIM_ID] = anim;

	anim = new Animation(ANIMATION_FAST_FRAMETIME);
	anim->Add(STARMAN_BIG2_WALK_SPRITE_1);
	anim->Add(STARMAN_BIG2_WALK_SPRITE_2);
	anim->Add(STARMAN_BIG2_WALK_SPRITE_3);
	starmanBlueprints[1][STARMAN_BIG1_RUN_ANIM_ID] = anim;

	anim = new Animation(ANIMATION_FAST_FRAMETIME);
	anim->Add(STARMAN_BIG3_WALK_SPRITE_1);
	anim->Add(STARMAN_BIG3_WALK_SPRITE_2);
	anim->Add(STARMAN_BIG3_WALK_SPRITE_3);
	starmanBlueprints[2][STARMAN_BIG1_RUN_ANIM_ID] = anim;
}

void Mario::LoadSpriteAndAnimation()
{

	LoadSmallNormalMario();
	LoadBigNormalMario();
	LoadFireMario();
	LoadSmallStarman();
	LoadBigStarman();
	LoadRaccoonMario();
	LoadStarmanPalette(currentStarmanAnimPalette);
}

void Mario::LoadRaccoonMario()
{
	auto marioTex = Textures::GetInstance()->Get(MARIO_TEX_ID);
	auto anims = Animations::GetInstance();
	auto sprites = Sprites::GetInstance();
	Animation* anim = nullptr;
	
	sprites->Add(RACCOON_IDLE_SPRITE_1, 8, 256, 31, 287, marioTex);
	sprites->Add(RACCOON_IDLE_SPRITE_2, 40, 256, 63, 287, marioTex);
	sprites->Add(RACCOON_IDLE_SPRITE_3, 72, 256, 95, 287, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(RACCOON_IDLE_SPRITE_1);
	anim->Add(RACCOON_IDLE_SPRITE_2);
	anim->Add(RACCOON_IDLE_SPRITE_3);
	anims->Add(RACCOON_IDLE_ANIM_ID, anim);

	sprites->Add(RACCOON_WALK_SPRITE_1, 104, 256, 127, 287, marioTex);
	sprites->Add(RACCOON_WALK_SPRITE_2, 136, 256, 159, 287, marioTex);
	sprites->Add(RACCOON_WALK_SPRITE_3, 168, 256, 191, 287, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(RACCOON_WALK_SPRITE_1);
	anim->Add(RACCOON_WALK_SPRITE_2);
	anim->Add(RACCOON_WALK_SPRITE_3);
	anims->Add(RACCOON_WALK_ANIM_ID, anim);

	anim = new Animation(ANIMATION_FAST_FRAMETIME);
	anim->Add(RACCOON_WALK_SPRITE_1);
	anim->Add(RACCOON_WALK_SPRITE_2);
	anim->Add(RACCOON_WALK_SPRITE_3);
	anims->Add(RACCOON_RUN_ANIM_ID, anim);

	sprites->Add(RACCOON_JUMP_SPRITE_1, 200, 256, 223, 287, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(RACCOON_JUMP_SPRITE_1);
	anims->Add(RACCOON_JUMP_ANIM_ID, anim);

	sprites->Add(RACCOON_SKID_SPRITE_1, 16, 288, 39, 319, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(RACCOON_SKID_SPRITE_1);
	anims->Add(RACCOON_SKID_ANIM_ID, anim);

	sprites->Add(RACCOON_DUCK_SPRITE_1, 40, 288, 63, 319, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(RACCOON_DUCK_SPRITE_1);
	anims->Add(RACCOON_DUCK_ANIM_ID, anim);

	sprites->Add(RACCOON_PREFLY_SPRITE_1, 8, 320, 31, 351, marioTex);
	sprites->Add(RACCOON_PREFLY_SPRITE_2, 40, 320, 63, 351, marioTex);
	sprites->Add(RACCOON_PREFLY_SPRITE_3, 72, 320, 95, 351, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(RACCOON_PREFLY_SPRITE_1);
	anim->Add(RACCOON_PREFLY_SPRITE_2);
	anim->Add(RACCOON_PREFLY_SPRITE_3);
	anims->Add(RACCOON_PREFLY_ANIM_ID, anim);

	sprites->Add(RACCOON_FLY_SPRITE_1, 72, 288, 95, 319, marioTex);
	sprites->Add(RACCOON_FLY_SPRITE_2, 104, 288, 127, 319, marioTex);
	sprites->Add(RACCOON_FLY_SPRITE_3, 136, 288, 159, 319, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(RACCOON_FLY_SPRITE_1);
	anim->Add(RACCOON_FLY_SPRITE_2);
	anim->Add(RACCOON_FLY_SPRITE_3);
	anims->Add(RACCOON_FLY_ANIM_ID, anim);

	sprites->Add(RACCOON_FLAGPULL_SPRITE_1, 168, 288, 191, 319, marioTex);
	sprites->Add(RACCOON_FLAGPULL_SPRITE_2, 200, 256, 223, 287, marioTex);
	anim = new Animation(ANIMATION_DEFAULT_FRAMETIME);
	anim->Add(RACCOON_FLAGPULL_SPRITE_1);
	anim->Add(RACCOON_FLAGPULL_SPRITE_2);
	anims->Add(RACCOON_FLAGPULL_ANIM_ID, anim);
}
