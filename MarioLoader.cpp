#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "Mario.h"
#include "Sprites.h"
#include "Textures.h"

void Mario::LoadSpriteAndAnimation()
{
	auto marioTex = Textures::GetInstance()->Get(MARIO_TEX_ID);

	auto anims = Animations::GetInstance();
	auto sprites = Sprites::GetInstance();

	/// ================================
	// NormalTransition sprites
	/// ================================
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

	// idle anim
	auto anim = new Animation(300);
	anim->Add(MARIO_IDLE_SPRITE_1);
	anims->Add(MARIO_IDLE_ANIM_ID, anim);

	// walk anim
	anim = new Animation(100);
	anim->Add(MARIO_RUN_SPRITE_1);
	anim->Add(MARIO_RUN_SPRITE_2);
	anim->Add(MARIO_RUN_SPRITE_3);
	anims->Add(MARIO_RUN_ANIM_ID, anim);

	// skid anim
	anim = new Animation(100);
	anim->Add(MARIO_SKID_SPRITE_1);
	anims->Add(MARIO_SKID_ANIM_ID, anim);

	// jump anim
	anim = new Animation(100);
	anim->Add(MARIO_JUMP_SPRITE_1);
	anims->Add(MARIO_JUMP_ANIM_ID, anim);

	// death anim
	anim = new Animation(100);
	anim->Add(MARIO_DEATH_SPRITE_1);
	anims->Add(MARIO_DEATH_ANIM_ID, anim);

	// grow to big anim
	anim = new Animation(150);
	anim->Add(MARIO_GROWBIG_SPRITE_1, 100);
	anim->Add(MARIO_GROWBIG_SPRITE_2, 100);
	anim->Add(MARIO_GROWBIG_SPRITE_1, 100);
	anim->Add(MARIO_GROWBIG_SPRITE_2, 100);
	anim->Add(MARIO_GROWBIG_SPRITE_3, 100);
	anim->Add(MARIO_GROWBIG_SPRITE_2, 100);
	anim->Add(MARIO_GROWBIG_SPRITE_3, 100);
	anims->Add(MARIO_GROWBIG_ANIM_ID, anim);

	// flag pull anim
	anim = new Animation(150);
	anim->Add(MARIO_FLAG_PULL_SPRITE_1, 150);
	anim->Add(MARIO_FLAG_PULL_SPRITE_2, 150);
	anims->Add(MARIO_FLAG_PULL_ANIM_ID, anim);

	/// ================================
	// BIG sprites
	/// ================================
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


	anim = new Animation(100);
	// idle anim
	anim->Add(MARIO_BIG_IDLE_SPRITE_1);
	anims->Add(MARIO_BIG_IDLE_ANIM_ID, anim);

	// walk anim
	anim = new Animation(100);
	anim->Add(MARIO_BIG_RUN_SPRITE_1);
	anim->Add(MARIO_BIG_RUN_SPRITE_2);
	anim->Add(MARIO_BIG_RUN_SPRITE_3);
	anims->Add(MARIO_BIG_RUN_ANIM_ID, anim);

	// skid anim
	anim = new Animation(100);
	anim->Add(MARIO_BIG_SKID_SPRITE_1);
	anims->Add(MARIO_BIG_SKID_ANIM_ID, anim);

	// jump anim
	anim = new Animation(100);
	anim->Add(MARIO_BIG_JUMP_SPRITE_1);
	anims->Add(MARIO_BIG_JUMP_ANIM_ID, anim);

	// duck anim
	anim = new Animation(100);
	anim->Add(MARIO_BIG_DUCK_SPRITE_1);
	anims->Add(MARIO_BIG_DUCK_ANIM_ID, anim);

	//shrink anim
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

	//flag pull anim
	anim = new Animation(150);
	anim->Add(MARIO_BIG_FLAG_PULL_SPRITE_1, 150);
	anim->Add(MARIO_BIG_FLAG_PULL_SPRITE_2, 150);
	anims->Add(MARIO_BIG_FLAG_PULL_ANIM_ID, anim);

	/// ================================
	// FIRE sprites
	/// ================================
	sprites->Add(MARIO_FIRE_IDLE_SPRITE_1, 0, 80, 15, 111, marioTex);

	sprites->Add(MARIO_FIRE_RUN_SPRITE_1, 16, 80, 31, 111, marioTex);
	sprites->Add(MARIO_FIRE_RUN_SPRITE_2, 32, 80, 47, 111, marioTex);
	sprites->Add(MARIO_FIRE_RUN_SPRITE_3, 48, 80, 63, 111, marioTex);
	sprites->Add(MARIO_FIRE_SKID_SPRITE_1, 64, 80, 79, 111, marioTex);

	sprites->Add(MARIO_FIRE_JUMP_SPRITE_1, 80, 80, 95, 111, marioTex);
	sprites->Add(MARIO_FIRE_DUCK_SPRITE_1, 96, 80, 111, 111, marioTex);

	sprites->Add(MARIO_FIRE_FIRE_SPRITE_1, 16, 80, 31, 111, marioTex);
	// Reusing the run sprite for firing since it's the same pose
	sprites->Add(MARIO_FIRE_FLAG_PULL_SPRITE_1, 112, 80, 127, 111, marioTex);
	sprites->Add(MARIO_FIRE_FLAG_PULL_SPRITE_2, 128, 80, 143, 111, marioTex);

	//idle anim
	anim = new Animation(300);
	anim->Add(MARIO_FIRE_IDLE_SPRITE_1);
	anims->Add(MARIO_FIRE_IDLE_ANIM_ID, anim);

	// walk anim
	anim = new Animation(100);
	anim->Add(MARIO_FIRE_RUN_SPRITE_1);
	anim->Add(MARIO_FIRE_RUN_SPRITE_2);
	anim->Add(MARIO_FIRE_RUN_SPRITE_3);
	anims->Add(MARIO_FIRE_RUN_ANIM_ID, anim);

	// skid anim
	anim = new Animation(100);
	anim->Add(MARIO_FIRE_SKID_SPRITE_1);
	anims->Add(MARIO_FIRE_SKID_ANIM_ID, anim);

	// jump anim
	anim = new Animation(100);
	anim->Add(MARIO_FIRE_JUMP_SPRITE_1);
	anims->Add(MARIO_FIRE_JUMP_ANIM_ID, anim);

	// duck anim
	anim = new Animation(100);
	anim->Add(MARIO_FIRE_DUCK_SPRITE_1);
	anims->Add(MARIO_FIRE_DUCK_ANIM_ID, anim);

	// fire anim
	anim = new Animation(300);
	anim->Add(MARIO_FIRE_FIRE_SPRITE_1);
	anims->Add(MARIO_FIRE_FIRE_ANIM_ID, anim);

	//flag pull anim
	anim = new Animation(150);
	anim->Add(MARIO_FIRE_FLAG_PULL_SPRITE_1, 150);
	anim->Add(MARIO_FIRE_FLAG_PULL_SPRITE_2, 150);
	anims->Add(MARIO_FIRE_FLAG_PULL_ANIM_ID, anim);
}
