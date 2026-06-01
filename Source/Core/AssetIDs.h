#pragma once

// [NOTE]: Negative values are reserved for playable scenes and scene backgrounds.

/// Naming convention:
/// Most asset IDs use 5 digits in the format ETAS.
/// E = Entity | T = Type/Variant | A = Action | S = Sprite/Frame
/// Example: 12345 = Mario (1) / Texture (2) / Action (3) / Sprite (45)
/// Entities above 9 use the same idea with one extra leading digit.
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

// Swimming (A = 900)
constexpr int MARIO_SWIM_ANIM_ID = 11900;
constexpr int MARIO_SWIM_SPRITE_1 = 11901;
constexpr int MARIO_SWIM_SPRITE_2 = 11902;
constexpr int MARIO_SWIM_SPRITE_3 = 11903;
constexpr int MARIO_SWIM_SPRITE_4 = 11904;
constexpr int MARIO_BREATH_BUBBLE = 11905;



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

// Swimming (A = 900)
constexpr int MARIO_BIG_SWIM_ANIM_ID = 12900;
constexpr int MARIO_BIG_SWIM_SPRITE_1 = 12901;
constexpr int MARIO_BIG_SWIM_SPRITE_2 = 12902;
constexpr int MARIO_BIG_SWIM_SPRITE_3 = 12903;
constexpr int MARIO_BIG_SWIM_SPRITE_4 = 12904;

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

// Swimming (A = 900)
constexpr int MARIO_FIRE_SWIM_ANIM_ID = 13900;
constexpr int MARIO_FIRE_SWIM_SPRITE_1 = 13901;
constexpr int MARIO_FIRE_SWIM_SPRITE_2 = 13902;
constexpr int MARIO_FIRE_SWIM_SPRITE_3 = 13903;
constexpr int MARIO_FIRE_SWIM_SPRITE_4 = 13904;

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

// Swimming (A = 900)
constexpr int STARMAN_SWIM_ANIM_ID = 16900;
constexpr int STARMAN_SWIM_SPRITE_1 = 16901;
constexpr int STARMAN_SWIM_SPRITE_2 = 16902;
constexpr int STARMAN_SWIM_SPRITE_3 = 16903;
constexpr int STARMAN_SWIM_SPRITE_4 = 16904;
constexpr int STARMAN_SWIM_SPRITE_5 = 16905;
constexpr int STARMAN_SWIM_SPRITE_6 = 16906;
constexpr int STARMAN_SWIM_SPRITE_7 = 16907;
constexpr int STARMAN_SWIM_SPRITE_8 = 16908;
constexpr int STARMAN_SWIM_SPRITE_9 = 16909;
constexpr int STARMAN_SWIM_SPRITE_10 = 16910;
constexpr int STARMAN_SWIM_SPRITE_11 = 16911;
constexpr int STARMAN_SWIM_SPRITE_12 = 16912;


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

// Swimming (A = 900)
constexpr int STARMAN_BIG_SWIM_ANIM_ID = 17900;
constexpr int STARMAN_BIG_SWIM_SPRITE_1 = 17901;
constexpr int STARMAN_BIG_SWIM_SPRITE_2 = 17902;
constexpr int STARMAN_BIG_SWIM_SPRITE_3 = 17903;
constexpr int STARMAN_BIG_SWIM_SPRITE_4 = 17904;
constexpr int STARMAN_BIG_SWIM_SPRITE_5 = 17905;
constexpr int STARMAN_BIG_SWIM_SPRITE_6 = 17906;
constexpr int STARMAN_BIG_SWIM_SPRITE_7 = 17907;
constexpr int STARMAN_BIG_SWIM_SPRITE_8 = 17908;
constexpr int STARMAN_BIG_SWIM_SPRITE_9 = 17909;
constexpr int STARMAN_BIG_SWIM_SPRITE_10 = 17910;
constexpr int STARMAN_BIG_SWIM_SPRITE_11 = 17911;
constexpr int STARMAN_BIG_SWIM_SPRITE_12 = 17912;

// ==========================================
// ENEMIES (E = 2)
// ==========================================
constexpr int OVERWORLD_ENEMY_TEX_ID = 20000;
constexpr int UNDERGROUND_ENEMY_TEX_ID = 20001;
constexpr int CASTLE_ENEMY_TEX_ID = 20002;
constexpr int UNDERWATER_ENEMY_TEX_ID = 20003;

// ---------------------------------
// GOOMBA (T = 1)
// ---------------------------------
// Moving (A = 200)
constexpr int GOOMBA_WALK_ANIM_ID = 21200;
constexpr int GOOMBA_WALK_SPRITE_1 = 21201;
constexpr int GOOMBA_WALK_SPRITE_2 = 21202;

// Dead (A = 700)
constexpr int GOOMBA_DEAD_ANIM_ID = 21700;
constexpr int GOOMBA_DEAD_SPRITE_1 = 21701;

// ---------------------------------
// KOOPA (T = 2)
// ---------------------------------
// Moving (A = 200)
constexpr int KOOPA_WALK_ANIM_ID = 22200;
constexpr int KOOPA_WALK_SPRITE_1 = 22201;
constexpr int KOOPA_WALK_SPRITE_2 = 22202;

// ---------------------------------
// WINGED KOOPA (T = 3)
// ---------------------------------
// Moving/Flying (A = 200)
constexpr int WINGED_KOOPA_FLY_ANIM_ID = 23200;
constexpr int WINGED_KOOPA_FLY_SPRITE_1 = 23201;
constexpr int WINGED_KOOPA_FLY_SPRITE_2 = 23202;

// ---------------------------------
// HIDING KOOPA (T = 4)
// ---------------------------------
// Idle/Hiding (A = 100)
constexpr int HIDING_KOOPA_HIDE_ANIM_ID = 24100;
constexpr int HIDING_KOOPA_HIDE_SPRITE = 24101;

// Moving/Spinning (A = 200)
constexpr int HIDING_KOOPA_SPIN_ANIM_ID = 24200;
constexpr int HIDING_KOOPA_SPIN_SPRITE_1 = 24201;
constexpr int HIDING_KOOPA_SPIN_SPRITE_2 = 24202;

// Dead (A = 700)
constexpr int KOOPA_DEAD_ANIM_ID = 24700;
constexpr int KOOPA_DEAD_SPRITE = 24701;

// ---------------------------------
// CHEEP CHEEPS (T = 5)
// ---------------------------------
// Red (A = 100)
constexpr int RED_CHEEP_ANIM = 25100;
constexpr int RED_CHEEP_SPRITE_1 = 25101;
constexpr int RED_CHEEP_SPRITE_2 = 25102;

// Water (A = 200)
constexpr int WATER_CHEEP_ANIM = 25200;
constexpr int WATER_CHEEP_SPRITE_1 = 25201;
constexpr int WATER_CHEEP_SPRITE_2 = 25202;

// ---------------------------------
// BLOOPERS (T = 6)
// ---------------------------------
// Swim (A = 100)
constexpr int WATER_BLOOPER_SWIM_ANIM = 26100;
constexpr int WATER_BLOOPER_SWIM_SPRITE_1 = 26101;

// Idle (A = 200)
constexpr int WATER_BLOOPER_IDLE_ANIM = 26200;
constexpr int WATER_BLOOPER_IDLE_SPRITE_1 = 26201;


// ==========================================
// BLOCKS (E = 3)
// ==========================================
constexpr int BLOCKS_OVERWORLD_TEX_ID = 30000;
constexpr int BLOCKS_UNDERGROUND_TEX_ID = 30001;
constexpr int BLOCKS_CASTLE_TEX_ID = 30002;
constexpr int BLOCKS_UNDERWATER_TEX_ID = 30003;

// ---------------------------------
// QUESTION BLOCK (T = 1)
// ---------------------------------
// Idle (A = 100)
constexpr int QUESTION_BLOCK_IDLE_ANIM_ID = 31100;
constexpr int QUESTION_BLOCK_IDLE_SPRITE_1 = 31101;
constexpr int QUESTION_BLOCK_IDLE_SPRITE_2 = 31102;
constexpr int QUESTION_BLOCK_IDLE_SPRITE_3 = 31103;

// Blocked/Empty (A = 700)
constexpr int BLOCK_EMPTY_ANIM_ID = 31700;
constexpr int BLOCK_EMPTY_SPRITE_1 = 31701;

// ---------------------------------
// BRICK (T = 2)
// ---------------------------------
// Idle (A = 100)
constexpr int BRICK_IDLE_ANIM_ID = 32100;
constexpr int BRICK_IDLE_SPRITE_1 = 32101;

// Explode (A = 700)
constexpr int BRICK_EXPLODE_SPRITE_1 = 32701;
constexpr int BRICK_EXPLODE_SPRITE_2 = 32702;
constexpr int BRICK_EXPLODE_SPRITE_3 = 32703;
constexpr int BRICK_EXPLODE_SPRITE_4 = 32704;

// Bridge (T = 4) 
constexpr int BRIDGE_ANIM_ID = 34100;
constexpr int BRIDGE_SPRITE_1 = 34101;

// ==========================================
// ITEMS (E = 4)
// ==========================================
constexpr int OVERWORLD_ITEMS_TEX_ID = 40000;
constexpr int UNDERGROUND_ITEMS_TEX_ID = 40001;
constexpr int CASTLE_ITEMS_TEX_ID = 40002;
constexpr int UNDERWATER_ITEMS_TEX_ID = 40003;

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

// Crown / Jetpack (T = 6) -> Idle (A = 100)
constexpr int CROWN_TEX_ID = 46000;
constexpr int CROWN_IDLE_ANIM_ID = 46100;
constexpr int CROWN_WEAR_ANIM_ID = 46110;
constexpr int CROWN_BIG_SPRITE = 46101;
constexpr int CROWN_SMALL_SPRITE = 46102;
//Axe (T = 7) 
constexpr int AXE_ANIM_ID = 47100;
constexpr int AXE_SPRITE_1 = 47101;


// ==========================================
// HUD ELEMENTS (E = 5)
// ==========================================
constexpr int HUDCOIN_TEX_ID = 50000;
constexpr int HUDCOIN_SPRITE_ID = 51101;
constexpr int HUDMARIO_SPRITE_ID = 52101;


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

// Collision/Explode (A = 300)
constexpr int FIREBALL_COLLIDE_ANIM_ID = 61300;
constexpr int FIREBALL_COLLIDE_SPRITE_1 = 61301;
constexpr int FIREBALL_COLLIDE_SPRITE_2 = 61302;
constexpr int FIREBALL_COLLIDE_SPRITE_3 = 61303;

// Castle FLAG
constexpr int CASTLE_FLAG_SPRITE_1 = 61401;



// ==========================================
// BOWSER (E = 8)
// ==========================================
constexpr int BOWSER_TEX_ID = 80000; // Fixed from 81000 to prevent internal overlap

// WALKING (A = 100)
constexpr int BOWSER_WALK_ANIM_ID = 81100;
constexpr int BOWSER_WALK_SPRITE_1 = 81101;
constexpr int BOWSER_WALK_SPRITE_2 = 81102;

// FIRE BREATHING (A = 300)
constexpr int BOWSER_FIRE_BREATH_ANIM_ID = 81300;
constexpr int BOWSER_FIRE_BREATH_SPRITE_1 = 81301;
constexpr int BOWSER_FIRE_BREATH_SPRITE_2 = 81302;

// HAMMER THROWING (A = 400)
constexpr int BOWSER_HAMMER_THROW_ANIM_ID = 81410;
constexpr int BOWSER_HAMMER_THROW_SPRITE = 81411;

//DYING (A = 500)
constexpr int BOWSER_DEATH_ANIM_ID = 81500;
constexpr int BOWSER_DEATH_SPRITE_1 = 81501;

// FALLING (A = 600)
constexpr int BOWSER_FALL_ANIM_ID = 81600;
constexpr int BOWSER_FALL_SPRITE_1 = 81601;
constexpr int BOWSER_FALL_SPRITE_2 = 81602;
constexpr int BOWSER_FALL_SPRITE_3 = 81603;


// ==========================================
// BOWSER ITEM BULLET (E = 9)  <-- FIXED from 100000 to prevent overlap with Points
// ==========================================
constexpr int BOWSER_ITEM_BULLET_TEX_ID = 90000;

// Fire breath (T = 1) -> Moving (A = 200)
constexpr int BOWSER_ITEM_BULLET_FIRE_ANIM_ID = 91200;
constexpr int BOWSER_ITEM_BULLET_FIRE_SPRITE_1 = 91201;
constexpr int BOWSER_ITEM_BULLET_FIRE_SPRITE_2 = 91202;

// Hammer (T = 2) -> Moving (A = 200)
constexpr int BOWSER_ITEM_BULLET_HAMMER_ANIM_ID = 92200;
constexpr int BOWSER_ITEM_BULLET_HAMMER_SPRITE_1 = 92201;
constexpr int BOWSER_ITEM_BULLET_HAMMER_SPRITE_2 = 92202;
constexpr int BOWSER_ITEM_BULLET_HAMMER_SPRITE_3 = 92203;
constexpr int BOWSER_ITEM_BULLET_HAMMER_SPRITE_4 = 92204;


// ==========================================
// POINT POPUPS (E = 10
// ==========================================
constexpr int POINTS_TEX_ID = 100000;

constexpr int POINT_100_ANIM_ID = 101100;
constexpr int POINT_100_SPRITE_1 = 101101;

constexpr int POINT_200_ANIM_ID = 102100;
constexpr int POINT_200_SPRITE_1 = 102101;

constexpr int POINT_400_ANIM_ID = 103100;
constexpr int POINT_400_SPRITE_1 = 103101;

constexpr int POINT_500_ANIM_ID = 104100;
constexpr int POINT_500_SPRITE_1 = 104101;

constexpr int POINT_800_ANIM_ID = 105100;
constexpr int POINT_800_SPRITE_1 = 105101;

constexpr int POINT_1000_ANIM_ID = 106100;
constexpr int POINT_1000_SPRITE_1 = 106101;

constexpr int POINT_2000_ANIM_ID = 107100;
constexpr int POINT_2000_SPRITE_1 = 107101;

constexpr int POINT_4000_ANIM_ID = 108100;
constexpr int POINT_4000_SPRITE_1 = 108101;

constexpr int POINT_5000_ANIM_ID = 109100;
constexpr int POINT_5000_SPRITE_1 = 109101;

constexpr int POINT_8000_ANIM_ID = 110100;
constexpr int POINT_8000_SPRITE_1 = 110101;

// 1-UP takes next entity slot natively in format (E = 11)
constexpr int ONEUP_ANIM_ID = 111100;
constexpr int ONEUP_SPRITE_1 = 111101;


// ==========================================
// SCENES & AUDIO
// ==========================================
/// PLAYABLE SCENES IS MANAGE BY LEVEL LOADER (INIT FUNCTION)
/// NEGATIVE VALUE (AND 0) IS FOR PLAYABLE SCENE ONLY
enum NonPlayableScene : std::int8_t
{
    MAIN_MENU = 1,
    GAME_OVER_SCENE = 2,
    ABOUT_SCENE = 3,
    LEVEL_TRANSITION = 99,
};

// ALL BACKGROUND MUSIC WILL BE MANAGED BY NEGATIVE VALUE AND 0
enum AUDIOS : std::int8_t
{
    GROUND_THEME = -3,
    UNDERGROUND_THEME = -2,
    CASTLE_THEME = -1,
    UNDERWATER_THEME = 0,


    // SFX 
    MARIO_JUMP_SMALL = 1,
    MARIO_JUMP_BIG = 2,
    GOOMBA_STOMP = 3,
    MARIO_DIE = 4,
    FIREBALL = 5,
    MARIO_COLLECT_COIN = 6,
    MARIO_BUMP = 7,
    MARIO_POWERUP = 8,
    POWERUP_APPEARS = 9,
    BREAK_BLOCK = 10,
    PIPE_ENTER = 11,
    INVINCIBILITY_THEME = 12,
    FLAG_PULL = 13,
    STAGE_CLEAR = 14,
    FIREWORK = 15,
    GAMEOVER = 16,
};

/// FONT
enum FONTS : std::uint8_t
{
    STATS_FONT = 1,
    HUD_FONT = 2,
    WORLD_FONT = 3,
    WORLD_FONT_BOLD,
    WORLD_FONT_ITALIC,
    WORLD_FONT_BOLD_ITALIC,

};
