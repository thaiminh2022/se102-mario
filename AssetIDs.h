#pragma once

// [NOTE]: NEGATIVE VALUES IS RESERVE FOR SCENE BACKGROUND

/// Naming convention:
/// Each asset ID uses 5 digits in the format ETAS.
/// E = Entity | T = Type/Texture | A = Action | S = Sprite
/// Example: 12345 = Mario (1) / Texture (2) / Action (3) / Sprite (45)
///
/// STANDARD ACTION IDs (A):
/// 1 = Idle    | 2 = Move   | 3 = Skid  | 4 = Jump | 5 = Duck 
/// 6 = Attack  | 7 = Dead   | 8 = Flag  | 9 = Transform

#include <cinttypes>

// ==========================================
// MARIO (E = 1)
// ==========================================
// ---------------------------------
// SMALL MARIO (T = 1)
// ---------------------------------
constexpr int MARIO_TEX_ID = 10000;
// Idle (A = 100)
constexpr int MARIO_IDLE_ANIM_ID = 11100;
constexpr int MARIO_IDLE_SPRITE_1 = 11101;
// Moving (A = 200)
constexpr int MARIO_RUN_ANIM_ID = 11200;
constexpr int MARIO_RUN_SPRITE_1 = 11201;
constexpr int MARIO_RUN_SPRITE_2 = 11202;
constexpr int MARIO_RUN_SPRITE_3 = 11203;
// Skidding (A = 300)
constexpr int MARIO_SKID_ANIM_ID = 11300;
constexpr int MARIO_SKID_SPRITE_1 = 11301;
// Jumping (A = 400)
constexpr int MARIO_JUMP_ANIM_ID = 11400;
constexpr int MARIO_JUMP_SPRITE_1 = 11401;
// Dying (A = 700)
constexpr int MARIO_DEATH_ANIM_ID = 11700;
constexpr int MARIO_DEATH_SPRITE_1 = 11701;
// Flag pulling (A = 800)
constexpr int MARIO_FLAG_PULL_ANIM_ID = 11800;
constexpr int MARIO_FLAG_PULL_SPRITE_1 = 11801;
constexpr int MARIO_FLAG_PULL_SPRITE_2 = 11802;

// ---------------------------------
// BIG MARIO (T = 2)
// ---------------------------------
// Idle (A = 100)
constexpr int MARIO_BIG_IDLE_ANIM_ID = 12100;
constexpr int MARIO_BIG_IDLE_SPRITE_1 = 12101;
// Moving (A = 200)
constexpr int MARIO_BIG_RUN_ANIM_ID = 12200;
constexpr int MARIO_BIG_RUN_SPRITE_1 = 12201;
constexpr int MARIO_BIG_RUN_SPRITE_2 = 12202;
constexpr int MARIO_BIG_RUN_SPRITE_3 = 12203;
// Skidding (A = 300)
constexpr int MARIO_BIG_SKID_ANIM_ID = 12300;
constexpr int MARIO_BIG_SKID_SPRITE_1 = 12301;
// Jumping (A = 400)
constexpr int MARIO_BIG_JUMP_ANIM_ID = 12400;
constexpr int MARIO_BIG_JUMP_SPRITE_1 = 12401;
// Ducking (A = 500)
constexpr int MARIO_BIG_DUCK_ANIM_ID = 12500;
constexpr int MARIO_BIG_DUCK_SPRITE_1 = 12501;
// Flag pulling (A = 800)
constexpr int MARIO_BIG_FLAG_PULL_ANIM_ID = 12800;
constexpr int MARIO_BIG_FLAG_PULL_SPRITE_1 = 12801;
constexpr int MARIO_BIG_FLAG_PULL_SPRITE_2 = 12802;

// ---------------------------------
// FIRE MARIO (T = 3)
// ---------------------------------
// Idle (A = 100)
constexpr int MARIO_FIRE_IDLE_ANIM_ID = 13100;
constexpr int MARIO_FIRE_IDLE_SPRITE_1 = 13101;
// Moving (A = 200)
constexpr int MARIO_FIRE_RUN_ANIM_ID = 13200;
constexpr int MARIO_FIRE_RUN_SPRITE_1 = 13201;
constexpr int MARIO_FIRE_RUN_SPRITE_2 = 13202;
constexpr int MARIO_FIRE_RUN_SPRITE_3 = 13203;
// Skidding (A = 300)
constexpr int MARIO_FIRE_SKID_ANIM_ID = 13300;
constexpr int MARIO_FIRE_SKID_SPRITE_1 = 13301;
// Jumping (A = 400)
constexpr int MARIO_FIRE_JUMP_ANIM_ID = 13400;
constexpr int MARIO_FIRE_JUMP_SPRITE_1 = 13401;
// Ducking (A = 500)
constexpr int MARIO_FIRE_DUCK_ANIM_ID = 13500;
constexpr int MARIO_FIRE_DUCK_SPRITE_1 = 13501;
// Firing (A = 600)
constexpr int MARIO_FIRE_FIRE_ANIM_ID = 13600;
constexpr int MARIO_FIRE_FIRE_SPRITE_1 = 13601;
// Flag pulling (A = 800)
constexpr int MARIO_FIRE_FLAG_PULL_ANIM_ID = 13800;
constexpr int MARIO_FIRE_FLAG_PULL_SPRITE_1 = 13801;
constexpr int MARIO_FIRE_FLAG_PULL_SPRITE_2 = 13802;

// ---------------------------------
// TRANSITION STATES (T = 4 and T = 5)
// ---------------------------------
// Growing (Small -> Big) (T = 4, A = 900)
constexpr int MARIO_GROWBIG_ANIM_ID = 14900;
constexpr int MARIO_GROWBIG_SPRITE_1 = 14901;
constexpr int MARIO_GROWBIG_SPRITE_2 = 14902;
constexpr int MARIO_GROWBIG_SPRITE_3 = 14903;
// Shrinking (Big -> Small) (T = 5, A = 900)
constexpr int MARIO_SHRINK_ANIM_ID = 15900;
constexpr int MARIO_SHRINK_SPRITE_1 = 15901;
constexpr int MARIO_SHRINK_SPRITE_2 = 15902;
constexpr int MARIO_SHRINK_SPRITE_3 = 15903;

// ---------------------------------
// SMALL STARMAN (T = 6)
// ---------------------------------
// Idle (A = 100)
constexpr int STARMAN_SMALL_IDLE_ANIM_ID = 16100;
constexpr int STARMAN_SMALL_IDLE_SPRITE_1 = 16101;
constexpr int STARMAN_SMALL_IDLE_SPRITE_2 = 16102;
constexpr int STARMAN_SMALL_IDLE_SPRITE_3 = 16103;
// Moving (A = 200)
constexpr int STARMAN_SMALL_RUN_ANIM_ID = 16200;
constexpr int STARMAN_SMALL_RUN_SPRITE_1 = 16201;
constexpr int STARMAN_SMALL_RUN_SPRITE_2 = 16202;
constexpr int STARMAN_SMALL_RUN_SPRITE_3 = 16203;
constexpr int STARMAN_SMALL_RUN_SPRITE_4 = 16204;
constexpr int STARMAN_SMALL_RUN_SPRITE_5 = 16205;
constexpr int STARMAN_SMALL_RUN_SPRITE_6 = 16206;
constexpr int STARMAN_SMALL_RUN_SPRITE_7 = 16207;
constexpr int STARMAN_SMALL_RUN_SPRITE_8 = 16208;
constexpr int STARMAN_SMALL_RUN_SPRITE_9 = 16209;
// Skidding (A = 300)
constexpr int STARMAN_SMALL_SKID_ANIM_ID = 16300;
constexpr int STARMAN_SMALL_SKID_SPRITE_1 = 16301;
constexpr int STARMAN_SMALL_SKID_SPRITE_2 = 16302;
constexpr int STARMAN_SMALL_SKID_SPRITE_3 = 16303;
// Jumping (A = 400)
constexpr int STARMAN_SMALL_JUMP_ANIM_ID = 16400;
constexpr int STARMAN_SMALL_JUMP_SPRITE_1 = 16401;
constexpr int STARMAN_SMALL_JUMP_SPRITE_2 = 16402;
constexpr int STARMAN_SMALL_JUMP_SPRITE_3 = 16403;
// Flag pulling (A = 800)
constexpr int STARMAN_SMALL_FLAG_PULL_ANIM_ID = 16800;
constexpr int STARMAN_SMALL_FLAG_PULL_SPRITE_1 = 16801;
constexpr int STARMAN_SMALL_FLAG_PULL_SPRITE_2 = 16802;
constexpr int STARMAN_SMALL_FLAG_PULL_SPRITE_3 = 16803;
constexpr int STARMAN_SMALL_FLAG_PULL_SPRITE_4 = 16804;
constexpr int STARMAN_SMALL_FLAG_PULL_SPRITE_5 = 16805;
constexpr int STARMAN_SMALL_FLAG_PULL_SPRITE_6 = 16806;
// ---------------------------------
// BIG STARMAN (T = 7)
// ---------------------------------
// Idle (A = 100)
constexpr int STARMAN_BIG_IDLE_ANIM_ID = 17100;
constexpr int STARMAN_BIG_IDLE_SPRITE_1 = 17101;
constexpr int STARMAN_BIG_IDLE_SPRITE_2 = 17102;
constexpr int STARMAN_BIG_IDLE_SPRITE_3 = 17103;
// Moving (A = 200)
constexpr int STARMAN_BIG_RUN_ANIM_ID = 17200;
constexpr int STARMAN_BIG_RUN_SPRITE_1 = 17201;
constexpr int STARMAN_BIG_RUN_SPRITE_2 = 17202;
constexpr int STARMAN_BIG_RUN_SPRITE_3 = 17203;
constexpr int STARMAN_BIG_RUN_SPRITE_4 = 17204;
constexpr int STARMAN_BIG_RUN_SPRITE_5 = 17205;
constexpr int STARMAN_BIG_RUN_SPRITE_6 = 17206;
constexpr int STARMAN_BIG_RUN_SPRITE_7 = 17207;
constexpr int STARMAN_BIG_RUN_SPRITE_8 = 17208;
constexpr int STARMAN_BIG_RUN_SPRITE_9 = 17209;
// Skidding (A = 300)
constexpr int STARMAN_BIG_SKID_ANIM_ID = 17300;
constexpr int STARMAN_BIG_SKID_SPRITE_1 = 17301;
constexpr int STARMAN_BIG_SKID_SPRITE_2 = 17302;
constexpr int STARMAN_BIG_SKID_SPRITE_3 = 17303;
// Jumping (A = 400)
constexpr int STARMAN_BIG_JUMP_ANIM_ID = 17400;
constexpr int STARMAN_BIG_JUMP_SPRITE_1 = 17401;
constexpr int STARMAN_BIG_JUMP_SPRITE_2 = 17402;
constexpr int STARMAN_BIG_JUMP_SPRITE_3 = 17403;
// Ducking (A = 500)
constexpr int STARMAN_BIG_DUCK_ANIM_ID = 17500;
constexpr int STARMAN_BIG_DUCK_SPRITE_1 = 17501;
constexpr int STARMAN_BIG_DUCK_SPRITE_2 = 17502;
constexpr int STARMAN_BIG_DUCK_SPRITE_3 = 17503;
// Flag pulling (A = 800)
constexpr int STARMAN_BIG_FLAG_PULL_ANIM_ID = 17800;
constexpr int STARMAN_BIG_FLAG_PULL_SPRITE_1 = 17801;
constexpr int STARMAN_BIG_FLAG_PULL_SPRITE_2 = 17802;
constexpr int STARMAN_BIG_FLAG_PULL_SPRITE_3 = 17803;
constexpr int STARMAN_BIG_FLAG_PULL_SPRITE_4 = 17804;
constexpr int STARMAN_BIG_FLAG_PULL_SPRITE_5 = 17805;
constexpr int STARMAN_BIG_FLAG_PULL_SPRITE_6 = 17806;

// ==========================================
// GOOMBA (E = 2)
// ==========================================
constexpr int GOOMBA_TEX_ID = 20000;

// Moving (A = 200)
constexpr int GOOMBA_WALK_ANIM_ID = 21200;
constexpr int GOOMBA_WALK_SPRITE_1 = 21201;
constexpr int GOOMBA_WALK_SPRITE_2 = 21202;

// Dead (A = 700)
constexpr int GOOMBA_DEAD_ANIM_ID = 21700;
constexpr int GOOMBA_DEAD_SPRITE_1 = 21701;


// ==========================================
// BLOCKS (E = 3)
// ==========================================
constexpr int BLOCKS_OVERWORLD_TEX_ID = 30000;

// ---------------------------------
// QUESTION BLOCK (T = 1)
// ---------------------------------
// Idle (A = 100)
constexpr int QUESTION_BLOCK_OVERWORLD_IDLE_ANIM_ID = 31100;
constexpr int QUESTION_BLOCK_OVERWORLD_IDLE_SPRITE_1 = 31101;
constexpr int QUESTION_BLOCK_OVERWORLD_IDLE_SPRITE_2 = 31102;
constexpr int QUESTION_BLOCK_OVERWORLD_IDLE_SPRITE_3 = 31103;

// Blocked/Empty (A = 700)
constexpr int BLOCK_OVERWORLD_BLOCKED_ANIM_ID = 31700;
constexpr int BLOCK_OVERWORLD_BLOCKED_SPRITE_1 = 31701;

// ---------------------------------
// BRICK (T = 2)
// ---------------------------------
// Idle (A = 100)
constexpr int BRICK_OVERWORLD_IDLE_ANIM_ID = 32100;
constexpr int BRICK_OVERWORLD_IDLE_SPRITE_1 = 32101;

// Explode (A = 700)
constexpr int BRICK_EXPLODE_SPRITE_1 = 32701;
constexpr int BRICK_EXPLODE_SPRITE_2 = 32702;
constexpr int BRICK_EXPLODE_SPRITE_3 = 32703;
constexpr int BRICK_EXPLODE_SPRITE_4 = 32704;

// ==========================================
// POINT POPUPS (E = 9) - MOVED TO 90000 RANGE
// ==========================================
constexpr int POINTS_TEX_ID = 90000;

constexpr int POINT_100_ANIM_ID = 90100;
constexpr int POINT_100_SPRITE_1 = 90101;

constexpr int POINT_200_ANIM_ID = 90200;
constexpr int POINT_200_SPRITE_1 = 90201;

constexpr int POINT_400_ANIM_ID = 90400;
constexpr int POINT_400_SPRITE_1 = 90401;

constexpr int POINT_500_ANIM_ID = 90500;
constexpr int POINT_500_SPRITE_1 = 90501;

constexpr int POINT_800_ANIM_ID = 90800;
constexpr int POINT_800_SPRITE_1 = 90801;

constexpr int POINT_1000_ANIM_ID = 91000;
constexpr int POINT_1000_SPRITE_1 = 91001;

constexpr int POINT_2000_ANIM_ID = 92000;
constexpr int POINT_2000_SPRITE_1 = 92001;

constexpr int POINT_4000_ANIM_ID = 94000;
constexpr int POINT_4000_SPRITE_1 = 94001;

constexpr int POINT_5000_ANIM_ID = 95000;
constexpr int POINT_5000_SPRITE_1 = 95001;

constexpr int POINT_8000_ANIM_ID = 98000;
constexpr int POINT_8000_SPRITE_1 = 98001;

constexpr int ONEUP_ANIM_ID = 99000;
constexpr int ONEUP_SPRITE_1 = 99001;


// ==========================================
// ITEMS (E = 4)
// ==========================================
constexpr int OVERWORLD_ITEMS_TEX_ID = 40000;

// Coin (T = 1) -> Moving/Spinning (A = 200)
constexpr int COIN_SPIN_ANIM_ID = 41200;
constexpr int COIN_SPIN_SPRITE_1 = 41201;
constexpr int COIN_SPIN_SPRITE_2 = 41202;
constexpr int COIN_SPIN_SPRITE_3 = 41203;
constexpr int COIN_SPIN_SPRITE_4 = 41204;

// Mushroom (T = 2) -> Moving (A = 200)
constexpr int MUSHROOM_ANIM_ID = 42200;
constexpr int MUSHROOM_SPRITE_1 = 42201;

// Flower (T = 3) -> Idle (A = 100)
constexpr int FLOWER_IDLE_ANIM_ID = 43100;
constexpr int FLOWER_IDLE_SPRITE_1 = 43101;
constexpr int FLOWER_IDLE_SPRITE_2 = 43102;
constexpr int FLOWER_IDLE_SPRITE_3 = 43103;
constexpr int FLOWER_IDLE_SPRITE_4 = 43104;

// Star (T = 4) -> Idle (A = 100)
constexpr int STAR_IDLE_ANIM_ID = 44100;
constexpr int STAR_IDLE_SPRITE_1 = 44101;
constexpr int STAR_IDLE_SPRITE_2 = 44102;
constexpr int STAR_IDLE_SPRITE_3 = 44103;
constexpr int STAR_IDLE_SPRITE_4 = 44104;

// Flag (T = 5) -> Idle (A = 100)
constexpr int FLAG_IDLE_ANIM_ID = 45100;
constexpr int FLAG_IDLE_SPRITE_1 = 45101;


// ==========================================
// FIREBALL (E = 6)
// ==========================================
constexpr int FIREBALL_TEX_ID = 60000;

// Moving/Bouncing (A = 200)
constexpr int FIREBALL_BOUNCE_ANIM_ID = 61200;
constexpr int FIREBALL_BOUNCE_SPRITE_1 = 61201;
constexpr int FIREBALL_BOUNCE_SPRITE_2 = 61202;
constexpr int FIREBALL_BOUNCE_SPRITE_3 = 61203;
constexpr int FIREBALL_BOUNCE_SPRITE_4 = 61204;

// Collision/Explode (A = 700)
constexpr int FIREBALL_COLLIDE_ANIM_ID = 61700;
constexpr int FIREBALL_COLLIDE_SPRITE_1 = 61701;
constexpr int FIREBALL_COLLIDE_SPRITE_2 = 61702;
constexpr int FIREBALL_COLLIDE_SPRITE_3 = 61703;


// ==========================================
// KOOPA (E = 7)
// ==========================================

// ---------------------------------
// NORMAL KOOPA (T = 1)
// ---------------------------------
constexpr int KOOPA_TEX_ID = 71000;

// Moving (A = 200)
constexpr int KOOPA_WALK_ANIM_ID = 71200;
constexpr int KOOPA_WALK_SPRITE_1 = 71201;
constexpr int KOOPA_WALK_SPRITE_2 = 71202;

// ---------------------------------
// WINGED KOOPA (T = 2)
// ---------------------------------
constexpr int WINGED_KOOPA_TEX_ID = 72000;

// Moving/Flying (A = 200)
constexpr int WINGED_KOOPA_FLY_ANIM_ID = 72200;
constexpr int WINGED_KOOPA_FLY_SPRITE_1 = 72201;
constexpr int WINGED_KOOPA_FLY_SPRITE_2 = 72202;

// ---------------------------------
// HIDING KOOPA (T = 3)
// ---------------------------------
constexpr int HIDING_KOOPA_TEX_ID = 73000;

// Idle/Hiding (A = 100)
constexpr int HIDING_KOOPA_HIDE_ANIM_ID = 73100;
constexpr int HIDING_KOOPA_HIDE_SPRITE = 73101;

// Moving/Spinning (A = 200)
constexpr int HIDING_KOOPA_SPIN_ANIM_ID = 73200;
constexpr int HIDING_KOOPA_SPIN_SPRITE_1 = 73201;
constexpr int HIDING_KOOPA_SPIN_SPRITE_2 = 73202;

// Dead (A = 700)
constexpr int KOOPA_DEAD_ANIM_ID = 73700;
constexpr int KOOPA_DEAD_SPRITE = 73701;


// ==========================================
// SCENES AND UI
// ==========================================

// HUD ELEMENTS
constexpr int HUDCOIN_TEX_ID = 50000;
constexpr int HUDCOIN_SPRITE_ID = 50001;
constexpr int HUDMARIO_SPRITE_ID = 51000;
constexpr int CASTLE_FLAG_TEX_ID = 52000;

// PLAYABLE SCENES IS MANAGE BY LEVEL LOADER (INIT FUNCTION)
// NEGATIVE VALUE (AND 0) IS FOR PLAYABLE SCENE ONLY
enum NonPlayableScene : std::int8_t
{
	MAIN_MENU = 1,
	LEVEL_TRANSITION = 99,
};

/// MUSICS: Background music will be managed by negative values in ldtk

// SFX
enum SFX : std::uint8_t
{
	MARIO_JUMP_SMALL = 1,
	MARIO_JUMP_BIG,
	GOOMBA_STOMP,
	MARIO_DIE,
	FIREBALL,
	MARIO_COLLECT_COIN,
	MARIO_BUMP,
	MARIO_POWERUP,
	POWERUP_APPEARS,
	BREAK_BLOCK,
	PIPE_ENTER,
	INVINCIBILITY_THEME,
	FLAG_PULL,
	STAGE_CLEAR,
	FIREWORKS,
	TING,
};

/// FONT
enum FONTS : std::uint8_t
{
	STATS_FONT = 1,
	HUD_FONT = 2
};
