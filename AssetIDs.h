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


/// ================================
/// MARIO (E = 10000)
/// ================================

// ---------------------------------
// SMALL MARIO (T = 1)
// ---------------------------------
constexpr int MARIO_TEX_ID = 11000;

// Idle (A = 100)
constexpr int MARIO_IDLE_ANIM_ID = 11100;
constexpr int MARIO_IDLE_SPRITE_1 = 11101;

// Running (A = 200)
constexpr int MARIO_RUN_ANIM_ID = 11200;
constexpr int MARIO_RUN_SPRITE_1 = 11201;
constexpr int MARIO_RUN_SPRITE_2 = 11202;
constexpr int MARIO_RUN_SPRITE_3 = 11203;

// SKIDDING(A = 300)
constexpr int MARIO_SKID_ANIM_ID = 11300;
constexpr int MARIO_SKID_SPRITE_1 = 11301;

// Jumping (A = 400)
constexpr int MARIO_JUMP_ANIM_ID = 11400;
constexpr int MARIO_JUMP_SPRITE_1 = 11401;

// Dying (A = 700)
constexpr int MARIO_DEATH_ANIM_ID = 11700;
constexpr int MARIO_DEATH_SPRITE_1 = 11701;


// ---------------------------------
// BIG MARIO (T = 2)
// ---------------------------------
constexpr int MARIO_BIG_TEX_ID = 12000;

// Idle (A = 100)
constexpr int MARIO_BIG_IDLE_ANIM_ID = 12100;
constexpr int MARIO_BIG_IDLE_SPRITE_1 = 12101;

// Running (A = 200)
constexpr int MARIO_BIG_RUN_ANIM_ID = 12200;
constexpr int MARIO_BIG_RUN_SPRITE_1 = 12201;
constexpr int MARIO_BIG_RUN_SPRITE_2 = 12202;
constexpr int MARIO_BIG_RUN_SPRITE_3 = 12203;

// SKIDDING (A = 300)
constexpr int MARIO_BIG_SKID_ANIM_ID = 12300;
constexpr int MARIO_BIG_SKID_SPRITE_1 = 12301;

// Jumping (A = 400)
constexpr int MARIO_BIG_JUMP_ANIM_ID = 12400;
constexpr int MARIO_BIG_JUMP_SPRITE_1 = 12401;

// Ducking (A = 500)
constexpr int MARIO_BIG_DUCK_ANIM_ID = 12500;
constexpr int MARIO_BIG_DUCK_SPRITE_1 = 12501;


// ---------------------------------
// FIRE MARIO (T = 3)
// ---------------------------------
constexpr int MARIO_FIRE_TEX_ID = 13000;

// Idle (A = 100)
constexpr int MARIO_FIRE_IDLE_ANIM_ID = 13100;
constexpr int MARIO_FIRE_IDLE_SPRITE_1 = 13101;

// Running (A = 200)
constexpr int MARIO_FIRE_RUN_ANIM_ID = 13200;
constexpr int MARIO_FIRE_RUN_SPRITE_1 = 13201;
constexpr int MARIO_FIRE_RUN_SPRITE_2 = 13202;
constexpr int MARIO_FIRE_RUN_SPRITE_3 = 13203;

// SKIDing Around (A = 300)
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

/// ================================
// FIREBALL (E = 30000, T = 1)
/// ================================
constexpr int FIREBALL_TEX_ID = 31000;
// Bouncing (A = 100)
constexpr int FIREBALL_BOUNCE_ANIM_ID = 31100;
constexpr int FIREBALL_BOUNCE_SPRITE_1 = 31101;
constexpr int FIREBALL_BOUNCE_SPRITE_2 = 31102;
constexpr int FIREBALL_BOUNCE_SPRITE_3 = 31103;
constexpr int FIREBALL_BOUNCE_SPRITE_4 = 31104;

// Collision (A = 200)
constexpr int FIREBALL_COLLIDE_ANIM_ID = 31200;
constexpr int FIREBALL_COLLIDE_SPRITE_1 = 31201;
constexpr int FIREBALL_COLLIDE_SPRITE_2 = 31202;
constexpr int FIREBALL_COLLIDE_SPRITE_3 = 31203;
// star anim id (A = 4)
constexpr int STAR_IDLE_ANIM_ID = 41400;
constexpr int STAR_IDLE_SPRITE_1 = 41401;
constexpr int STAR_IDLE_SPRITE_2 = 41402;
constexpr int STAR_IDLE_SPRITE_3 = 41403;
constexpr int STAR_IDLE_SPRITE_4 = 41404;


/// MUSICS: Background music will be managed by negative values in ldtk
/// SFX

constexpr int MARIO_JUMP_SMALL = 1;
constexpr int MARIO_JUMP_BIG = 2;
constexpr int GOOMBA_STOMP = 3;
constexpr int MARIO_DIE = 4;
constexpr int FIREBALL = 5;
constexpr int MARIO_COLLECT_COIN = 5;
constexpr int MARIO_BUMP = 6;
constexpr int MARIO_POWERUP = 7;
constexpr int POWERUP_APPEARS = 8;



/// FONT
constexpr int STATS_FONT = 1;
