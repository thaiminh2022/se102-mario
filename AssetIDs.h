#pragma once

// [NOTE]: NEGATIVE VALUES IS RESERVE FOR SCENE BACKGROUND

/// Naming convention:
/// Each asset ID uses 5 digits in the format ETAS.
///
/// E = Entity
/// T = Texture
/// A = Animation
/// S = Sprite
///
/// Example:
/// 12345 = Mario / Texture 2 / Animation 3 / Sprite 45
///
/// This can be read as:
/// 10000 + 2000 + 300 + 40 + 5

#include <cinttypes>



// ---------------------------------
// SMALL MARIO (T = 1)
// ---------------------------------
constexpr int MARIO_TEX_ID = 10000;



// Idle (A = 100)
constexpr int MARIO_IDLE_ANIM_ID = 11100;
constexpr int MARIO_IDLE_SPRITE_1 = 11101;

// Running (A = 200)
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

//Flag pulling (A = 800)
constexpr int MARIO_FLAG_PULL_ANIM_ID = 11800;
constexpr int MARIO_FLAG_PULL_SPRITE_1 = 11801;
constexpr int MARIO_FLAG_PULL_SPRITE_2 = 11802;

// ==========================================
// BIG MARIO 
// ==========================================

// Idle (A = 100)
constexpr int MARIO_BIG_IDLE_ANIM_ID = 12100;
constexpr int MARIO_BIG_IDLE_SPRITE_1 = 12101;

// Running (A = 200)
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

// ==========================================
// FIRE MARIO 
// ==========================================

// Idle (A = 100)
constexpr int MARIO_FIRE_IDLE_ANIM_ID = 13100;
constexpr int MARIO_FIRE_IDLE_SPRITE_1 = 13101;

// Running (A = 200)
constexpr int MARIO_FIRE_RUN_ANIM_ID = 13200;
constexpr int MARIO_FIRE_RUN_SPRITE_1 = 13201;
constexpr int MARIO_FIRE_RUN_SPRITE_2 = 13202;
constexpr int MARIO_FIRE_RUN_SPRITE_3 = 13203;

/// Skidding (A = 300)

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

// ==========================================
// TRANSITION STATES 
// ==========================================

// Growing (Small -> Big) (T = 4, A = 100)
constexpr int MARIO_GROWBIG_ANIM_ID = 14100;
constexpr int MARIO_GROWBIG_SPRITE_1 = 14101;
constexpr int MARIO_GROWBIG_SPRITE_2 = 14102;
constexpr int MARIO_GROWBIG_SPRITE_3 = 14103;

// Shrinking (Big -> Small) (T = 5, A = 100)
constexpr int MARIO_SHRINK_ANIM_ID = 15100;
constexpr int MARIO_SHRINK_SPRITE_1 = 15101;
constexpr int MARIO_SHRINK_SPRITE_2 = 15102;
constexpr int MARIO_SHRINK_SPRITE_3 = 15103;


/// ================================
/// GOOMBA (E = 20000, T = 1)
/// ================================
constexpr int GOOMBA_TEX_ID = 21000;

// Walking (A = 100)
constexpr int GOOMBA_WALK_ANIM_ID = 21100;
constexpr int GOOMBA_WALK_SPRITE_1 = 21101;
constexpr int GOOMBA_WALK_SPRITE_2 = 21102;

// Dead (A = 200)
constexpr int GOOMBA_DEAD_ANIM_ID = 21200;
constexpr int GOOMBA_DEAD_SPRITE_1 = 21201;


/// ================================

// Question blocks + brick (E = 3000, T = 1 (overworld))

constexpr int BLOCKS_OVERWORLD_TEX_ID = 31000;

// Question block Idle (flashing color) (A = 100)

constexpr int QUESTION_BLOCK_OVERWORLD_IDLE_ANIM_ID = 31100;
constexpr int QUESTION_BLOCK_OVERWORLD_IDLE_SPRITE_1 = 31101;
constexpr int QUESTION_BLOCK_OVERWORLD_IDLE_SPRITE_2 = 31102;
constexpr int QUESTION_BLOCK_OVERWORLD_IDLE_SPRITE_3 = 31103;

// Blocked (block is opened by player) (A = 200)

constexpr int BLOCK_OVERWORLD_BLOCKED_ANIM_ID = 31200;
constexpr int BLOCK_OVERWORLD_BLOCKED_SPRITE_1 = 31201;

// Brick (A = 300)

constexpr int BRICK_OVERWORLD_IDLE_ANIM_ID = 31300;
constexpr int BRICK_OVERWORLD_IDLE_SPRITE_1 = 31301;

// Brick Explode (A = 400) // animation index doesn't matter but use anyway lol

constexpr int BRICK_EXPLODE_SPRITE_1 = 31401;
constexpr int BRICK_EXPLODE_SPRITE_2 = 31402;
constexpr int BRICK_EXPLODE_SPRITE_3 = 31403;
constexpr int BRICK_EXPLODE_SPRITE_4 = 31404;

/// ================================

// Items (E=4000, T=1 (overworld))
constexpr int OVERWORLD_ITEMS_TEX_ID = 41000;

// coin anim (A = 1)
constexpr int COIN_SPIN_ANIM_ID = 41100;
constexpr int COIN_SPIN_SPRITE_1= 41101;
constexpr int COIN_SPIN_SPRITE_2 = 41102;
constexpr int COIN_SPIN_SPRITE_3 = 41103;
constexpr int COIN_SPIN_SPRITE_4 = 41104;

// mushroom anim (A = 2)
constexpr int MUSHROOM_ANIM_ID = 41200;
constexpr int MUSHROOM_SPRITE_1 = 41201;

// flower anim id (A = 3)
constexpr int FLOWER_IDLE_ANIM_ID = 41300;
constexpr int FLOWER_IDLE_SPRITE_1 = 41301;
constexpr int FLOWER_IDLE_SPRITE_2 = 41302;
constexpr int FLOWER_IDLE_SPRITE_3 = 41303;
constexpr int FLOWER_IDLE_SPRITE_4 = 41304;

// star anim id (A = 4)
constexpr int STAR_IDLE_ANIM_ID = 41400;
constexpr int STAR_IDLE_SPRITE_1 = 41401;
constexpr int STAR_IDLE_SPRITE_2 = 41402;
constexpr int STAR_IDLE_SPRITE_3 = 41403;
constexpr int STAR_IDLE_SPRITE_4 = 41404;

// flag
constexpr int FLAG_IDLE_ANIM_ID = 41500;
constexpr int FLAG_IDLE_SPRITE_1 = 41501;


/// ================================
// FIREBALL (E = 60000, T = 1)
/// ================================
constexpr int FIREBALL_TEX_ID =	61000;
// Bouncing (A = 100)
constexpr int FIREBALL_BOUNCE_ANIM_ID = 61100;
constexpr int FIREBALL_BOUNCE_SPRITE_1 = 61101;
constexpr int FIREBALL_BOUNCE_SPRITE_2 = 61102;
constexpr int FIREBALL_BOUNCE_SPRITE_3 = 61103;
constexpr int FIREBALL_BOUNCE_SPRITE_4 = 61104;

// Collision (A = 200)
constexpr int FIREBALL_COLLIDE_ANIM_ID = 61200;
constexpr int FIREBALL_COLLIDE_SPRITE_1 = 61201;
constexpr int FIREBALL_COLLIDE_SPRITE_2 = 61202;
constexpr int FIREBALL_COLLIDE_SPRITE_3 = 61203;

/// ================================
// KOOPA (E = 70000)
/// ================================

/// ================================ 
//  NORMAL KOOPA (T = 1)
/// ================================
constexpr int KOOPA_TEX_ID = 71000;

// Walking (A = 100)
constexpr int KOOPA_WALK_ANIM_ID = 71100;
constexpr int KOOPA_WALK_SPRITE_1 = 71101;
constexpr int KOOPA_WALK_SPRITE_2 = 71102;


/// ===============================
// WINGED KOOPA (T = 2)
/// ===============================
constexpr int WINGED_KOOPA_TEX_ID = 72000;

// Winged (A = 100)
constexpr int WINGED_KOOPA_FLY_ANIM_ID = 72100;
constexpr int WINGED_KOOPA_FLY_SPRITE_1 = 72101;
constexpr int WINGED_KOOPA_FLY_SPRITE_2 = 72102;

/// ===============================
// HIDING KOOPA (T = 3)
/// ===============================
constexpr int HIDING_KOOPA_TEX_ID = 73000;

// Hiding (A = 100)
constexpr int HIDING_KOOPA_HIDE_ANIM_ID = 73100;
constexpr int HIDING_KOOPA_HIDE_SPRITE = 73101;

// Spinning (A = 200)
constexpr int HIDING_KOOPA_SPIN_ANIM_ID = 73200;
constexpr int HIDING_KOOPA_SPIN_SPRITE_1 = 73201;
constexpr int HIDING_KOOPA_SPIN_SPRITE_2 = 73202;

// Died (A = 300)
constexpr int KOOPA_DEAD_ANIM_ID = 73300;
constexpr int KOOPA_DEAD_SPRITE = 73301;

/// ===============================
// BOWSER (E = 80000, T = 1)
/// ===============================
constexpr int BOWSER_TEX_ID = 81000;

// WALKING (A = 100)
constexpr int BOWSER_WALK_ANIM_ID = 81100;
constexpr int BOWSER_WALK_SPRITE_1 = 81101;
constexpr int BOWSER_WALK_SPRITE_2 = 81102;

// JUMPING (A = 200)
constexpr int BOWSER_JUMP_ANIM_ID = 81200;
constexpr int BOWSER_JUMP_SPRITE_1 = 81201;

// FIRE BREATHING (A = 300)
constexpr int BOWSER_FIRE_BREATH_ANIM_ID = 81300;
constexpr int BOWSER_FIRE_BREATH_SPRITE_1 = 81301;
constexpr int BOWSER_FIRE_BREATH_SPRITE_2 = 81302;

// HAMMER THROWING (A = 400)
constexpr int BOWSER_HAMMER_THROW_LEFT_ANIM_ID = 81410;
constexpr int BOWSER_HAMMER_THROW_LEFT_SPRITE = 81411;

constexpr int BOWSER_HAMMER_THROW_RIGHT_ANIM_ID = 81420;
constexpr int BOWSER_HAMMER_THROW_RIGHT_SPRITE = 81421;

//DYING (A = 500)
constexpr int BOWSER_DEATH_ANIM_ID = 81500;
constexpr int BOWSER_DEATH_SPRITE_1 = 81501;

// FALLING (A = 600)
constexpr int BOWSER_FALL_ANIM_ID = 81600;
constexpr int BOWSER_FALL_SPRITE_1 = 81601;
constexpr int BOWSER_FALL_SPRITE_2 = 81602;
constexpr int BOWSER_FALL_SPRITE_3 = 81603;

/// ================================
// BOWSER ITEM BULLET (E = 90000)
///=================================
constexpr int BOWSER_ITEM_BULLET_TEX_ID = 90000;

// Fire breath (T = 1000)
constexpr int BOWSER_ITEM_BULLET_FIRE_ANIM_ID = 91000;
constexpr int BOWSER_ITEM_BULLET_FIRE_SPRITE_1 = 91001;
constexpr int BOWSER_ITEM_BULLET_FIRE_SPRITE_2 = 91002;

// Hammer (T = 2000)
constexpr int BOWSER_ITEM_BULLET_HAMMER_ANIM_ID = 92000;
constexpr int BOWSER_ITEM_BULLET_HAMMER_SPRITE_1 = 92001;
constexpr int BOWSER_ITEM_BULLET_HAMMER_SPRITE_2 = 92002;
constexpr int BOWSER_ITEM_BULLET_HAMMER_SPRITE_3 = 92003;
constexpr int BOWSER_ITEM_BULLET_HAMMER_SPRITE_4 = 92004;

// SCENES THAT ARE NOT PLAYABLE SCENE

/// ================================
// HUD ELEMENTS
/// ================================
constexpr int HUDCOIN_TEX_ID = 50000;
constexpr int HUDCOIN_SPRITE_ID = 50001;
constexpr int HUDMARIO_SPRITE_ID = 51000;


// PLAYABLE SCENES IS MANAGE BY LEVEL LOADER (INIT FUNCTION)
// NEGATIVE VALUE (AND 0) IS FOR PLAYABLE SCENE ONLY
enum NonPlayableScene : std::int8_t
{
	MAIN_MENU = 1,
	LEVEL_TRANSITION = 99,
};

/// MUSICS: Background music will be managed by negative values in ldtk

// SFX
enum SFX: std::uint8_t
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
};

/// FONT
enum FONTS: std::uint8_t
{
	STATS_FONT = 1,
	HUD_FONT = 2
};
