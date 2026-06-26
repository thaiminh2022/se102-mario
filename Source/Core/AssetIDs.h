#pragma once
#include "AssetID.h"

// [NOTE]: Negative values are reserved for playable scenes and scene backgrounds.

/// Naming convention:
/// Most asset IDs use 5 digits in the format ETAS.
/// E = Entity | T = Type/Variant | A = Action | S = Sprite/Frame
/// Example: 12345 = Mario (1) / Texture (2) / Action (3) / Sprite (45)
/// Entities above 9 use the same idea with one extra leading digit.
///
/// STANDARD ACTION IDs (A):
/// 0 = Run (Fast)| 1 = Idle   | 2 = Move/Walk | 3 = Skid | 4 = Jump | 5 = Duck 
/// 6 = Attack    | 7 = Dead   | 8 = Flag      | 9 = Transform/Swim

#include <cinttypes>


// ==========================================
// MARIO (E = 1)
// ==========================================

// ---------------------------------
// SMALL MARIO (T = 1)
// ---------------------------------
constexpr AssetID MARIO_TEX_ID = AssetID("MARIO_TEX_ID");
// Running (A = 000)
constexpr AssetID MARIO_RUN_ANIM_ID = AssetID("MARIO_RUN_ANIM_ID"); //reusing the same sprites but shorter deltatime
// Idle (A = 100)
constexpr AssetID MARIO_IDLE_ANIM_ID = AssetID("MARIO_IDLE_ANIM_ID");
constexpr AssetID MARIO_IDLE_SPRITE_1 = AssetID("MARIO_IDLE_SPRITE_1");
// Walking (A = 200)
constexpr AssetID MARIO_WALK_ANIM_ID = AssetID("MARIO_WALK_ANIM_ID");
constexpr AssetID MARIO_WALK_SPRITE_1 = AssetID("MARIO_WALK_SPRITE_1");
constexpr AssetID MARIO_WALK_SPRITE_2 = AssetID("MARIO_WALK_SPRITE_2");
constexpr AssetID MARIO_WALK_SPRITE_3 = AssetID("MARIO_WALK_SPRITE_3");
// Skidding (A = 300)
constexpr AssetID MARIO_SKID_ANIM_ID = AssetID("MARIO_SKID_ANIM_ID");
constexpr AssetID MARIO_SKID_SPRITE_1 = AssetID("MARIO_SKID_SPRITE_1");
// Jumping (A = 400)
constexpr AssetID MARIO_JUMP_ANIM_ID = AssetID("MARIO_JUMP_ANIM_ID");
constexpr AssetID MARIO_JUMP_SPRITE_1 = AssetID("MARIO_JUMP_SPRITE_1");
// Dying (A = 700)
constexpr AssetID MARIO_DEATH_ANIM_ID = AssetID("MARIO_DEATH_ANIM_ID");
constexpr AssetID MARIO_DEATH_SPRITE_1 = AssetID("MARIO_DEATH_SPRITE_1");
// Flag pulling (A = 800)
constexpr AssetID MARIO_FLAG_PULL_ANIM_ID = AssetID("MARIO_FLAG_PULL_ANIM_ID");
constexpr AssetID MARIO_FLAG_PULL_SPRITE_1 = AssetID("MARIO_FLAG_PULL_SPRITE_1");
constexpr AssetID MARIO_FLAG_PULL_SPRITE_2 = AssetID("MARIO_FLAG_PULL_SPRITE_2");
// Swimming (A = 900)
constexpr AssetID MARIO_SWIM_ANIM_ID = AssetID("MARIO_SWIM_ANIM_ID");
constexpr AssetID MARIO_SWIM_SPRITE_1 = AssetID("MARIO_SWIM_SPRITE_1");
constexpr AssetID MARIO_SWIM_SPRITE_2 = AssetID("MARIO_SWIM_SPRITE_2");
constexpr AssetID MARIO_SWIM_SPRITE_3 = AssetID("MARIO_SWIM_SPRITE_3");
constexpr AssetID MARIO_SWIM_SPRITE_4 = AssetID("MARIO_SWIM_SPRITE_4");
constexpr AssetID MARIO_BREATH_BUBBLE = AssetID("MARIO_BREATH_BUBBLE");

// ---------------------------------
// BIG MARIO (T = 2)
// ---------------------------------
// Running (A = 000)
constexpr AssetID MARIO_BIG_RUN_ANIM_ID = AssetID("MARIO_BIG_RUN_ANIM_ID"); //reusing the same sprites but shorter deltatime
// Idle (A = 100)
constexpr AssetID MARIO_BIG_IDLE_ANIM_ID = AssetID("MARIO_BIG_IDLE_ANIM_ID");
constexpr AssetID MARIO_BIG_IDLE_SPRITE_1 = AssetID("MARIO_BIG_IDLE_SPRITE_1");
// Walking (A = 200)
constexpr AssetID MARIO_BIG_WALK_ANIM_ID = AssetID("MARIO_BIG_WALK_ANIM_ID");
constexpr AssetID MARIO_BIG_WALK_SPRITE_1 = AssetID("MARIO_BIG_WALK_SPRITE_1");
constexpr AssetID MARIO_BIG_WALK_SPRITE_2 = AssetID("MARIO_BIG_WALK_SPRITE_2");
constexpr AssetID MARIO_BIG_WALK_SPRITE_3 = AssetID("MARIO_BIG_WALK_SPRITE_3");
// Skidding (A = 300)
constexpr AssetID MARIO_BIG_SKID_ANIM_ID = AssetID("MARIO_BIG_SKID_ANIM_ID");
constexpr AssetID MARIO_BIG_SKID_SPRITE_1 = AssetID("MARIO_BIG_SKID_SPRITE_1");
// Jumping (A = 400)
constexpr AssetID MARIO_BIG_JUMP_ANIM_ID = AssetID("MARIO_BIG_JUMP_ANIM_ID");
constexpr AssetID MARIO_BIG_JUMP_SPRITE_1 = AssetID("MARIO_BIG_JUMP_SPRITE_1");
// Ducking (A = 500)
constexpr AssetID MARIO_BIG_DUCK_ANIM_ID = AssetID("MARIO_BIG_DUCK_ANIM_ID");
constexpr AssetID MARIO_BIG_DUCK_SPRITE_1 = AssetID("MARIO_BIG_DUCK_SPRITE_1");
// Flag pulling (A = 800)
constexpr AssetID MARIO_BIG_FLAG_PULL_ANIM_ID = AssetID("MARIO_BIG_FLAG_PULL_ANIM_ID");
constexpr AssetID MARIO_BIG_FLAG_PULL_SPRITE_1 = AssetID("MARIO_BIG_FLAG_PULL_SPRITE_1");
constexpr AssetID MARIO_BIG_FLAG_PULL_SPRITE_2 = AssetID("MARIO_BIG_FLAG_PULL_SPRITE_2");
// Swimming (A = 900)
constexpr AssetID MARIO_BIG_SWIM_ANIM_ID = AssetID("MARIO_BIG_SWIM_ANIM_ID");
constexpr AssetID MARIO_BIG_SWIM_SPRITE_1 = AssetID("MARIO_BIG_SWIM_SPRITE_1");
constexpr AssetID MARIO_BIG_SWIM_SPRITE_2 = AssetID("MARIO_BIG_SWIM_SPRITE_2");
constexpr AssetID MARIO_BIG_SWIM_SPRITE_3 = AssetID("MARIO_BIG_SWIM_SPRITE_3");
constexpr AssetID MARIO_BIG_SWIM_SPRITE_4 = AssetID("MARIO_BIG_SWIM_SPRITE_4");

// ---------------------------------
// FIRE MARIO (T = 3)
// ---------------------------------
// Running (A = 000)
constexpr AssetID MARIO_FIRE_RUN_ANIM_ID = AssetID("MARIO_FIRE_RUN_ANIM_ID"); //reusing the same sprites but shorter deltatime
// Idle (A = 100)
constexpr AssetID MARIO_FIRE_IDLE_ANIM_ID = AssetID("MARIO_FIRE_IDLE_ANIM_ID");
constexpr AssetID MARIO_FIRE_IDLE_SPRITE_1 = AssetID("MARIO_FIRE_IDLE_SPRITE_1");
// Walking (A = 200)
constexpr AssetID MARIO_FIRE_WALK_ANIM_ID = AssetID("MARIO_FIRE_WALK_ANIM_ID");
constexpr AssetID MARIO_FIRE_WALK_SPRITE_1 = AssetID("MARIO_FIRE_WALK_SPRITE_1");
constexpr AssetID MARIO_FIRE_WALK_SPRITE_2 = AssetID("MARIO_FIRE_WALK_SPRITE_2");
constexpr AssetID MARIO_FIRE_WALK_SPRITE_3 = AssetID("MARIO_FIRE_WALK_SPRITE_3");
// Skidding (A = 300)
constexpr AssetID MARIO_FIRE_SKID_ANIM_ID = AssetID("MARIO_FIRE_SKID_ANIM_ID");
constexpr AssetID MARIO_FIRE_SKID_SPRITE_1 = AssetID("MARIO_FIRE_SKID_SPRITE_1");
// Jumping (A = 400)
constexpr AssetID MARIO_FIRE_JUMP_ANIM_ID = AssetID("MARIO_FIRE_JUMP_ANIM_ID");
constexpr AssetID MARIO_FIRE_JUMP_SPRITE_1 = AssetID("MARIO_FIRE_JUMP_SPRITE_1");
// Ducking (A = 500)
constexpr AssetID MARIO_FIRE_DUCK_ANIM_ID = AssetID("MARIO_FIRE_DUCK_ANIM_ID");
constexpr AssetID MARIO_FIRE_DUCK_SPRITE_1 = AssetID("MARIO_FIRE_DUCK_SPRITE_1");
// Firing (A = 600)
constexpr AssetID MARIO_FIRE_FIRE_ANIM_ID = AssetID("MARIO_FIRE_FIRE_ANIM_ID");
constexpr AssetID MARIO_FIRE_FIRE_SPRITE_1 = AssetID("MARIO_FIRE_FIRE_SPRITE_1");
// Flag pulling (A = 800)
constexpr AssetID MARIO_FIRE_FLAG_PULL_ANIM_ID = AssetID("MARIO_FIRE_FLAG_PULL_ANIM_ID");
constexpr AssetID MARIO_FIRE_FLAG_PULL_SPRITE_1 = AssetID("MARIO_FIRE_FLAG_PULL_SPRITE_1");
constexpr AssetID MARIO_FIRE_FLAG_PULL_SPRITE_2 = AssetID("MARIO_FIRE_FLAG_PULL_SPRITE_2");
// Swimming (A = 900)
constexpr AssetID MARIO_FIRE_SWIM_ANIM_ID = AssetID("MARIO_FIRE_SWIM_ANIM_ID");
constexpr AssetID MARIO_FIRE_SWIM_SPRITE_1 = AssetID("MARIO_FIRE_SWIM_SPRITE_1");
constexpr AssetID MARIO_FIRE_SWIM_SPRITE_2 = AssetID("MARIO_FIRE_SWIM_SPRITE_2");
constexpr AssetID MARIO_FIRE_SWIM_SPRITE_3 = AssetID("MARIO_FIRE_SWIM_SPRITE_3");
constexpr AssetID MARIO_FIRE_SWIM_SPRITE_4 = AssetID("MARIO_FIRE_SWIM_SPRITE_4");

// ---------------------------------
// TRANSITION STATES (T = 4 and T = 5)
// ---------------------------------
// Growing (Small -> Big) (T = 4, A = 900)
constexpr AssetID MARIO_GROWBIG_ANIM_ID = AssetID("MARIO_GROWBIG_ANIM_ID");
constexpr AssetID MARIO_GROWBIG_SPRITE_1 = AssetID("MARIO_GROWBIG_SPRITE_1");
constexpr AssetID MARIO_GROWBIG_SPRITE_2 = AssetID("MARIO_GROWBIG_SPRITE_2");
constexpr AssetID MARIO_GROWBIG_SPRITE_3 = AssetID("MARIO_GROWBIG_SPRITE_3");
// Shrinking (Big -> Small) (T = 5, A = 900)
constexpr AssetID MARIO_SHRINK_ANIM_ID = AssetID("MARIO_SHRINK_ANIM_ID");
constexpr AssetID MARIO_SHRINK_SPRITE_1 = AssetID("MARIO_SHRINK_SPRITE_1");
constexpr AssetID MARIO_SHRINK_SPRITE_2 = AssetID("MARIO_SHRINK_SPRITE_2");
constexpr AssetID MARIO_SHRINK_SPRITE_3 = AssetID("MARIO_SHRINK_SPRITE_3");

// =================================================================
// PALETTE 1 (OFFSET +00)
// =================================================================

// ---------------------------------
// SMALL STARMAN 1 (T = 6)
// ---------------------------------
// Running (A = 000)
constexpr AssetID STARMAN_SMALL1_RUN_ANIM_ID = AssetID("STARMAN_SMALL1_RUN_ANIM_ID"); //reusing the same sprites but shorter deltatime
// Idle (A = 100)
constexpr AssetID STARMAN_SMALL1_IDLE_ANIM_ID = AssetID("STARMAN_SMALL1_IDLE_ANIM_ID");
constexpr AssetID STARMAN_SMALL1_IDLE_SPRITE_1 = AssetID("STARMAN_SMALL1_IDLE_SPRITE_1");
// Walking (A = 200)
constexpr AssetID STARMAN_SMALL1_WALK_ANIM_ID = AssetID("STARMAN_SMALL1_WALK_ANIM_ID");
constexpr AssetID STARMAN_SMALL1_WALK_SPRITE_1 = AssetID("STARMAN_SMALL1_WALK_SPRITE_1");
constexpr AssetID STARMAN_SMALL1_WALK_SPRITE_2 = AssetID("STARMAN_SMALL1_WALK_SPRITE_2");
constexpr AssetID STARMAN_SMALL1_WALK_SPRITE_3 = AssetID("STARMAN_SMALL1_WALK_SPRITE_3");
// Skidding (A = 300)
constexpr AssetID STARMAN_SMALL1_SKID_ANIM_ID = AssetID("STARMAN_SMALL1_SKID_ANIM_ID");
constexpr AssetID STARMAN_SMALL1_SKID_SPRITE_1 = AssetID("STARMAN_SMALL1_SKID_SPRITE_1");
// Jumping (A = 400)
constexpr AssetID STARMAN_SMALL1_JUMP_ANIM_ID = AssetID("STARMAN_SMALL1_JUMP_ANIM_ID");
constexpr AssetID STARMAN_SMALL1_JUMP_SPRITE_1 = AssetID("STARMAN_SMALL1_JUMP_SPRITE_1");
// Flag pulling (A = 800)
constexpr AssetID STARMAN_SMALL1_FLAG_PULL_ANIM_ID = AssetID("STARMAN_SMALL1_FLAG_PULL_ANIM_ID");
constexpr AssetID STARMAN_SMALL1_FLAG_PULL_SPRITE_1 = AssetID("STARMAN_SMALL1_FLAG_PULL_SPRITE_1");
constexpr AssetID STARMAN_SMALL1_FLAG_PULL_SPRITE_2 = AssetID("STARMAN_SMALL1_FLAG_PULL_SPRITE_2");
// Swimming (A = 900)
constexpr AssetID STARMAN_SMALL1_SWIM_ANIM_ID = AssetID("STARMAN_SMALL1_SWIM_ANIM_ID");
constexpr AssetID STARMAN_SMALL1_SWIM_SPRITE_1 = AssetID("STARMAN_SMALL1_SWIM_SPRITE_1");
constexpr AssetID STARMAN_SMALL1_SWIM_SPRITE_2 = AssetID("STARMAN_SMALL1_SWIM_SPRITE_2");
constexpr AssetID STARMAN_SMALL1_SWIM_SPRITE_3 = AssetID("STARMAN_SMALL1_SWIM_SPRITE_3");
constexpr AssetID STARMAN_SMALL1_SWIM_SPRITE_4 = AssetID("STARMAN_SMALL1_SWIM_SPRITE_4");

// ---------------------------------
// BIG STARMAN 1 (T = 7)
// ---------------------------------
// Running (A = 000)
constexpr AssetID STARMAN_BIG1_RUN_ANIM_ID = AssetID("STARMAN_BIG1_RUN_ANIM_ID"); //reusing the same sprites but shorter deltatime
// Idle (A = 100)
constexpr AssetID STARMAN_BIG1_IDLE_ANIM_ID = AssetID("STARMAN_BIG1_IDLE_ANIM_ID");
constexpr AssetID STARMAN_BIG1_IDLE_SPRITE_1 = AssetID("STARMAN_BIG1_IDLE_SPRITE_1");
// Walking (A = 200)
constexpr AssetID STARMAN_BIG1_WALK_ANIM_ID = AssetID("STARMAN_BIG1_WALK_ANIM_ID");
constexpr AssetID STARMAN_BIG1_WALK_SPRITE_1 = AssetID("STARMAN_BIG1_WALK_SPRITE_1");
constexpr AssetID STARMAN_BIG1_WALK_SPRITE_2 = AssetID("STARMAN_BIG1_WALK_SPRITE_2");
constexpr AssetID STARMAN_BIG1_WALK_SPRITE_3 = AssetID("STARMAN_BIG1_WALK_SPRITE_3");
// Skidding (A = 300)
constexpr AssetID STARMAN_BIG1_SKID_ANIM_ID = AssetID("STARMAN_BIG1_SKID_ANIM_ID");
constexpr AssetID STARMAN_BIG1_SKID_SPRITE_1 = AssetID("STARMAN_BIG1_SKID_SPRITE_1");
// Jumping (A = 400)
constexpr AssetID STARMAN_BIG1_JUMP_ANIM_ID = AssetID("STARMAN_BIG1_JUMP_ANIM_ID");
constexpr AssetID STARMAN_BIG1_JUMP_SPRITE_1 = AssetID("STARMAN_BIG1_JUMP_SPRITE_1");
// Ducking (A = 500)
constexpr AssetID STARMAN_BIG1_DUCK_ANIM_ID = AssetID("STARMAN_BIG1_DUCK_ANIM_ID");
constexpr AssetID STARMAN_BIG1_DUCK_SPRITE_1 = AssetID("STARMAN_BIG1_DUCK_SPRITE_1");
// Flag pulling (A = 800)
constexpr AssetID STARMAN_BIG1_FLAG_PULL_ANIM_ID = AssetID("STARMAN_BIG1_FLAG_PULL_ANIM_ID");
constexpr AssetID STARMAN_BIG1_FLAG_PULL_SPRITE_1 = AssetID("STARMAN_BIG1_FLAG_PULL_SPRITE_1");
constexpr AssetID STARMAN_BIG1_FLAG_PULL_SPRITE_2 = AssetID("STARMAN_BIG1_FLAG_PULL_SPRITE_2");
// Swimming (A = 900)
constexpr AssetID STARMAN_BIG1_SWIM_ANIM_ID = AssetID("STARMAN_BIG1_SWIM_ANIM_ID");
constexpr AssetID STARMAN_BIG1_SWIM_SPRITE_1 = AssetID("STARMAN_BIG1_SWIM_SPRITE_1");
constexpr AssetID STARMAN_BIG1_SWIM_SPRITE_2 = AssetID("STARMAN_BIG1_SWIM_SPRITE_2");
constexpr AssetID STARMAN_BIG1_SWIM_SPRITE_3 = AssetID("STARMAN_BIG1_SWIM_SPRITE_3");
constexpr AssetID STARMAN_BIG1_SWIM_SPRITE_4 = AssetID("STARMAN_BIG1_SWIM_SPRITE_4");

// =================================================================
// PALETTE 2 (OFFSET +30)
// =================================================================

// ---------------------------------
// SMALL STARMAN 2 (T = 6)
// ---------------------------------

constexpr AssetID STARMAN_SMALL2_IDLE_SPRITE_1 = AssetID("STARMAN_SMALL2_IDLE_SPRITE_1");

constexpr AssetID STARMAN_SMALL2_WALK_SPRITE_1 = AssetID("STARMAN_SMALL2_WALK_SPRITE_1");
constexpr AssetID STARMAN_SMALL2_WALK_SPRITE_2 = AssetID("STARMAN_SMALL2_WALK_SPRITE_2");
constexpr AssetID STARMAN_SMALL2_WALK_SPRITE_3 = AssetID("STARMAN_SMALL2_WALK_SPRITE_3");

constexpr AssetID STARMAN_SMALL2_SKID_SPRITE_1 = AssetID("STARMAN_SMALL2_SKID_SPRITE_1");

constexpr AssetID STARMAN_SMALL2_JUMP_SPRITE_1 = AssetID("STARMAN_SMALL2_JUMP_SPRITE_1");

constexpr AssetID STARMAN_SMALL2_FLAG_PULL_SPRITE_1 = AssetID("STARMAN_SMALL2_FLAG_PULL_SPRITE_1");
constexpr AssetID STARMAN_SMALL2_FLAG_PULL_SPRITE_2 = AssetID("STARMAN_SMALL2_FLAG_PULL_SPRITE_2");

constexpr AssetID STARMAN_SMALL2_SWIM_SPRITE_1 = AssetID("STARMAN_SMALL2_SWIM_SPRITE_1");
constexpr AssetID STARMAN_SMALL2_SWIM_SPRITE_2 = AssetID("STARMAN_SMALL2_SWIM_SPRITE_2");
constexpr AssetID STARMAN_SMALL2_SWIM_SPRITE_3 = AssetID("STARMAN_SMALL2_SWIM_SPRITE_3");
constexpr AssetID STARMAN_SMALL2_SWIM_SPRITE_4 = AssetID("STARMAN_SMALL2_SWIM_SPRITE_4");

// ---------------------------------
// BIG STARMAN 2 (T = 7)
// ---------------------------------

constexpr AssetID STARMAN_BIG2_IDLE_SPRITE_1 = AssetID("STARMAN_BIG2_IDLE_SPRITE_1");

constexpr AssetID STARMAN_BIG2_WALK_SPRITE_1 = AssetID("STARMAN_BIG2_WALK_SPRITE_1");
constexpr AssetID STARMAN_BIG2_WALK_SPRITE_2 = AssetID("STARMAN_BIG2_WALK_SPRITE_2");
constexpr AssetID STARMAN_BIG2_WALK_SPRITE_3 = AssetID("STARMAN_BIG2_WALK_SPRITE_3");

constexpr AssetID STARMAN_BIG2_SKID_SPRITE_1 = AssetID("STARMAN_BIG2_SKID_SPRITE_1");

constexpr AssetID STARMAN_BIG2_JUMP_SPRITE_1 = AssetID("STARMAN_BIG2_JUMP_SPRITE_1");

constexpr AssetID STARMAN_BIG2_DUCK_SPRITE_1 = AssetID("STARMAN_BIG2_DUCK_SPRITE_1");

constexpr AssetID STARMAN_BIG2_FLAG_PULL_SPRITE_1 = AssetID("STARMAN_BIG2_FLAG_PULL_SPRITE_1");
constexpr AssetID STARMAN_BIG2_FLAG_PULL_SPRITE_2 = AssetID("STARMAN_BIG2_FLAG_PULL_SPRITE_2");

constexpr AssetID STARMAN_BIG2_SWIM_SPRITE_1 = AssetID("STARMAN_BIG2_SWIM_SPRITE_1");
constexpr AssetID STARMAN_BIG2_SWIM_SPRITE_2 = AssetID("STARMAN_BIG2_SWIM_SPRITE_2");
constexpr AssetID STARMAN_BIG2_SWIM_SPRITE_3 = AssetID("STARMAN_BIG2_SWIM_SPRITE_3");
constexpr AssetID STARMAN_BIG2_SWIM_SPRITE_4 = AssetID("STARMAN_BIG2_SWIM_SPRITE_4");


// =================================================================
// PALETTE 3 (OFFSET +60)
// =================================================================

// ---------------------------------
// SMALL STARMAN 3 (T = 6)
// ---------------------------------


constexpr AssetID STARMAN_SMALL3_IDLE_SPRITE_1 = AssetID("STARMAN_SMALL3_IDLE_SPRITE_1");

constexpr AssetID STARMAN_SMALL3_WALK_SPRITE_1 = AssetID("STARMAN_SMALL3_WALK_SPRITE_1");
constexpr AssetID STARMAN_SMALL3_WALK_SPRITE_2 = AssetID("STARMAN_SMALL3_WALK_SPRITE_2");
constexpr AssetID STARMAN_SMALL3_WALK_SPRITE_3 = AssetID("STARMAN_SMALL3_WALK_SPRITE_3");

constexpr AssetID STARMAN_SMALL3_SKID_SPRITE_1 = AssetID("STARMAN_SMALL3_SKID_SPRITE_1");

constexpr AssetID STARMAN_SMALL3_JUMP_SPRITE_1 = AssetID("STARMAN_SMALL3_JUMP_SPRITE_1");

constexpr AssetID STARMAN_SMALL3_FLAG_PULL_SPRITE_1 = AssetID("STARMAN_SMALL3_FLAG_PULL_SPRITE_1");
constexpr AssetID STARMAN_SMALL3_FLAG_PULL_SPRITE_2 = AssetID("STARMAN_SMALL3_FLAG_PULL_SPRITE_2");

constexpr AssetID STARMAN_SMALL3_SWIM_SPRITE_1 = AssetID("STARMAN_SMALL3_SWIM_SPRITE_1");
constexpr AssetID STARMAN_SMALL3_SWIM_SPRITE_2 = AssetID("STARMAN_SMALL3_SWIM_SPRITE_2");
constexpr AssetID STARMAN_SMALL3_SWIM_SPRITE_3 = AssetID("STARMAN_SMALL3_SWIM_SPRITE_3");
constexpr AssetID STARMAN_SMALL3_SWIM_SPRITE_4 = AssetID("STARMAN_SMALL3_SWIM_SPRITE_4");

// ---------------------------------
// BIG STARMAN 3 (T = 7)
// ---------------------------------

constexpr AssetID STARMAN_BIG3_IDLE_SPRITE_1 = AssetID("STARMAN_BIG3_IDLE_SPRITE_1");

constexpr AssetID STARMAN_BIG3_WALK_SPRITE_1 = AssetID("STARMAN_BIG3_WALK_SPRITE_1");
constexpr AssetID STARMAN_BIG3_WALK_SPRITE_2 = AssetID("STARMAN_BIG3_WALK_SPRITE_2");
constexpr AssetID STARMAN_BIG3_WALK_SPRITE_3 = AssetID("STARMAN_BIG3_WALK_SPRITE_3");

constexpr AssetID STARMAN_BIG3_SKID_SPRITE_1 = AssetID("STARMAN_BIG3_SKID_SPRITE_1");

constexpr AssetID STARMAN_BIG3_JUMP_SPRITE_1 = AssetID("STARMAN_BIG3_JUMP_SPRITE_1");

constexpr AssetID STARMAN_BIG3_DUCK_SPRITE_1 = AssetID("STARMAN_BIG3_DUCK_SPRITE_1");

constexpr AssetID STARMAN_BIG3_FLAG_PULL_SPRITE_1 = AssetID("STARMAN_BIG3_FLAG_PULL_SPRITE_1");
constexpr AssetID STARMAN_BIG3_FLAG_PULL_SPRITE_2 = AssetID("STARMAN_BIG3_FLAG_PULL_SPRITE_2");

constexpr AssetID STARMAN_BIG3_SWIM_SPRITE_1 = AssetID("STARMAN_BIG3_SWIM_SPRITE_1");
constexpr AssetID STARMAN_BIG3_SWIM_SPRITE_2 = AssetID("STARMAN_BIG3_SWIM_SPRITE_2");
constexpr AssetID STARMAN_BIG3_SWIM_SPRITE_3 = AssetID("STARMAN_BIG3_SWIM_SPRITE_3");
constexpr AssetID STARMAN_BIG3_SWIM_SPRITE_4 = AssetID("STARMAN_BIG3_SWIM_SPRITE_4");

// ---------------------------------
// RACCOON MARIO
// ---------------------------------
constexpr AssetID RACCOON_IDLE_ANIM_ID = AssetID("RACCOON_IDLE_ANIM_ID");
constexpr AssetID RACCOON_IDLE_SPRITE_1 = AssetID("RACCOON_IDLE_SPRITE_1");
constexpr AssetID RACCOON_IDLE_SPRITE_2 = AssetID("RACCOON_IDLE_SPRITE_2");
constexpr AssetID RACCOON_IDLE_SPRITE_3 = AssetID("RACCOON_IDLE_SPRITE_3");

constexpr AssetID RACCOON_WALK_ANIM_ID = AssetID("RACCOON_WALK_ANIM_ID");
constexpr AssetID RACCOON_WALK_SPRITE_1 = AssetID("RACCOON_WALK_SPRITE_1");
constexpr AssetID RACCOON_WALK_SPRITE_2 = AssetID("RACCOON_WALK_SPRITE_2");
constexpr AssetID RACCOON_WALK_SPRITE_3 = AssetID("RACCOON_WALK_SPRITE_3");

constexpr AssetID RACCOON_RUN_ANIM_ID = AssetID("RACCOON_RUN_ANIM_ID");

constexpr AssetID RACCOON_JUMP_ANIM_ID = AssetID("RACCOON_JUMP_ANIM_ID");
constexpr AssetID RACCOON_JUMP_SPRITE_1 = AssetID("RACCOON_JUMP_SPRITE_1");

constexpr AssetID RACCOON_SKID_ANIM_ID = AssetID("RACCOON_SKID_ANIM_ID");
constexpr AssetID RACCOON_SKID_SPRITE_1 = AssetID("RACCOON_SKID_SPRITE_1");

constexpr AssetID RACCOON_DUCK_ANIM_ID = AssetID("RACCOON_DUCK_ANIM_ID");
constexpr AssetID RACCOON_DUCK_SPRITE_1 = AssetID("RACCOON_DUCK_SPRITE_1");

constexpr AssetID RACCOON_FLY_ANIM_ID = AssetID("RACCOON_FLY_ANIM_ID");
constexpr AssetID RACCOON_FLY_SPRITE_1 = AssetID("RACCOON_FLY_SPRITE_1");
constexpr AssetID RACCOON_FLY_SPRITE_2 = AssetID("RACCOON_FLY_SPRITE_2");
constexpr AssetID RACCOON_FLY_SPRITE_3 = AssetID("RACCOON_FLY_SPRITE_3");

constexpr AssetID RACCOON_PREFLY_ANIM_ID = AssetID("RACCOON_PREFLY_ANIM_ID");
constexpr AssetID RACCOON_PREFLY_SPRITE_1 = AssetID("RACCOON_PREFLY_SPRITE_1");
constexpr AssetID RACCOON_PREFLY_SPRITE_2 = AssetID("RACCOON_PREFLY_SPRITE_2");
constexpr AssetID RACCOON_PREFLY_SPRITE_3 = AssetID("RACCOON_PREFLY_SPRITE_3");

constexpr AssetID RACCOON_FLAGPULL_ANIM_ID = AssetID("RACCOON_FLAGPULL_ANIM_ID");
constexpr AssetID RACCOON_FLAGPULL_SPRITE_1 = AssetID("RACCOON_FLAGPULL_SPRITE_1");
constexpr AssetID RACCOON_FLAGPULL_SPRITE_2 = AssetID("RACCOON_FLAGPULL_SPRITE_2");

// ==========================================
// ENEMIES (E = 2)
// ==========================================
constexpr AssetID OVERWORLD_ENEMY_TEX_ID = AssetID("OVERWORLD_ENEMY_TEX_ID");
constexpr AssetID UNDERGROUND_ENEMY_TEX_ID = AssetID("UNDERGROUND_ENEMY_TEX_ID");
constexpr AssetID CASTLE_ENEMY_TEX_ID = AssetID("CASTLE_ENEMY_TEX_ID");
constexpr AssetID UNDERWATER_ENEMY_TEX_ID = AssetID("UNDERWATER_ENEMY_TEX_ID");

// ---------------------------------
// GOOMBA (T = 1)
// ---------------------------------
// Walking (A = 200)
constexpr AssetID GOOMBA_WALK_ANIM_ID = AssetID("GOOMBA_WALK_ANIM_ID");
constexpr AssetID GOOMBA_WALK_SPRITE_1 = AssetID("GOOMBA_WALK_SPRITE_1");
constexpr AssetID GOOMBA_WALK_SPRITE_2 = AssetID("GOOMBA_WALK_SPRITE_2");

// Dead (A = 700)
constexpr AssetID GOOMBA_DEAD_ANIM_ID = AssetID("GOOMBA_DEAD_ANIM_ID");
constexpr AssetID GOOMBA_DEAD_SPRITE_1 = AssetID("GOOMBA_DEAD_SPRITE_1");

// ---------------------------------
// KOOPA (T = 2)
// ---------------------------------
// Walking (A = 200)
constexpr AssetID KOOPA_WALK_ANIM_ID = AssetID("KOOPA_WALK_ANIM_ID");
constexpr AssetID KOOPA_WALK_SPRITE_1 = AssetID("KOOPA_WALK_SPRITE_1");
constexpr AssetID KOOPA_WALK_SPRITE_2 = AssetID("KOOPA_WALK_SPRITE_2");

// ---------------------------------
// WINGED KOOPA (T = 3)
// ---------------------------------
// Walking/Flying (A = 200)
constexpr AssetID WINGED_KOOPA_FLY_ANIM_ID = AssetID("WINGED_KOOPA_FLY_ANIM_ID");
constexpr AssetID WINGED_KOOPA_FLY_SPRITE_1 = AssetID("WINGED_KOOPA_FLY_SPRITE_1");
constexpr AssetID WINGED_KOOPA_FLY_SPRITE_2 = AssetID("WINGED_KOOPA_FLY_SPRITE_2");

// ---------------------------------
// HIDING KOOPA (T = 4)
// ---------------------------------
// Idle/Hiding (A = 100)
constexpr AssetID HIDING_KOOPA_HIDE_ANIM_ID = AssetID("HIDING_KOOPA_HIDE_ANIM_ID");
constexpr AssetID HIDING_KOOPA_HIDE_SPRITE = AssetID("HIDING_KOOPA_HIDE_SPRITE");

// Walking/Spinning (A = 200)
constexpr AssetID HIDING_KOOPA_SPIN_ANIM_ID = AssetID("HIDING_KOOPA_SPIN_ANIM_ID");
constexpr AssetID HIDING_KOOPA_SPIN_SPRITE_1 = AssetID("HIDING_KOOPA_SPIN_SPRITE_1");
constexpr AssetID HIDING_KOOPA_SPIN_SPRITE_2 = AssetID("HIDING_KOOPA_SPIN_SPRITE_2");

// Dead (A = 700)
constexpr AssetID KOOPA_DEAD_ANIM_ID = AssetID("KOOPA_DEAD_ANIM_ID");
constexpr AssetID KOOPA_DEAD_SPRITE = AssetID("KOOPA_DEAD_SPRITE");

// ---------------------------------
// CHEEP CHEEPS (T = 5)
// ---------------------------------
// Red (A = 100)
constexpr AssetID RED_CHEEP_ANIM = AssetID("RED_CHEEP_ANIM");
constexpr AssetID RED_CHEEP_SPRITE_1 = AssetID("RED_CHEEP_SPRITE_1");
constexpr AssetID RED_CHEEP_SPRITE_2 = AssetID("RED_CHEEP_SPRITE_2");

// Water (A = 200)
constexpr AssetID WATER_CHEEP_ANIM = AssetID("WATER_CHEEP_ANIM");
constexpr AssetID WATER_CHEEP_SPRITE_1 = AssetID("WATER_CHEEP_SPRITE_1");
constexpr AssetID WATER_CHEEP_SPRITE_2 = AssetID("WATER_CHEEP_SPRITE_2");

// ---------------------------------
// BLOOPERS (T = 6)
// ---------------------------------
// Swim (A = 100)
constexpr AssetID WATER_BLOOPER_SWIM_ANIM = AssetID("WATER_BLOOPER_SWIM_ANIM");
constexpr AssetID WATER_BLOOPER_SWIM_SPRITE_1 = AssetID("WATER_BLOOPER_SWIM_SPRITE_1");

// Idle (A = 200)
constexpr AssetID WATER_BLOOPER_IDLE_ANIM = AssetID("WATER_BLOOPER_IDLE_ANIM");
constexpr AssetID WATER_BLOOPER_IDLE_SPRITE_1 = AssetID("WATER_BLOOPER_IDLE_SPRITE_1");


// ==========================================
// BLOCKS (E = 3)
// ==========================================
constexpr AssetID BLOCKS_OVERWORLD_TEX_ID = AssetID("BLOCKS_OVERWORLD_TEX_ID");
constexpr AssetID BLOCKS_UNDERGROUND_TEX_ID = AssetID("BLOCKS_UNDERGROUND_TEX_ID");
constexpr AssetID BLOCKS_CASTLE_TEX_ID = AssetID("BLOCKS_CASTLE_TEX_ID");
constexpr AssetID BLOCKS_UNDERWATER_TEX_ID = AssetID("BLOCKS_UNDERWATER_TEX_ID");

// ---------------------------------
// QUESTION BLOCK (T = 1)
// ---------------------------------
// Idle (A = 100)
constexpr AssetID QUESTION_BLOCK_IDLE_ANIM_ID = AssetID("QUESTION_BLOCK_IDLE_ANIM_ID");
constexpr AssetID QUESTION_BLOCK_IDLE_SPRITE_1 = AssetID("QUESTION_BLOCK_IDLE_SPRITE_1");
constexpr AssetID QUESTION_BLOCK_IDLE_SPRITE_2 = AssetID("QUESTION_BLOCK_IDLE_SPRITE_2");
constexpr AssetID QUESTION_BLOCK_IDLE_SPRITE_3 = AssetID("QUESTION_BLOCK_IDLE_SPRITE_3");

// Blocked/Empty (A = 700)
constexpr AssetID BLOCK_EMPTY_ANIM_ID = AssetID("BLOCK_EMPTY_ANIM_ID");
constexpr AssetID BLOCK_EMPTY_SPRITE_1 = AssetID("BLOCK_EMPTY_SPRITE_1");

// ---------------------------------
// BRICK (T = 2)
// ---------------------------------
// Idle (A = 100)
constexpr AssetID BRICK_IDLE_ANIM_ID = AssetID("BRICK_IDLE_ANIM_ID");
constexpr AssetID BRICK_IDLE_SPRITE_1 = AssetID("BRICK_IDLE_SPRITE_1");

// Explode (A = 700)
constexpr AssetID BRICK_EXPLODE_SPRITE_1 = AssetID("BRICK_EXPLODE_SPRITE_1");
constexpr AssetID BRICK_EXPLODE_SPRITE_2 = AssetID("BRICK_EXPLODE_SPRITE_2");
constexpr AssetID BRICK_EXPLODE_SPRITE_3 = AssetID("BRICK_EXPLODE_SPRITE_3");
constexpr AssetID BRICK_EXPLODE_SPRITE_4 = AssetID("BRICK_EXPLODE_SPRITE_4");

// Bridge (T = 4) 
constexpr AssetID BRIDGE_ANIM_ID = AssetID("BRIDGE_ANIM_ID");
constexpr AssetID BRIDGE_SPRITE_1 = AssetID("BRIDGE_SPRITE_1");

// ==========================================
// ITEMS (E = 4)
// ==========================================
constexpr AssetID OVERWORLD_ITEMS_TEX_ID = AssetID("OVERWORLD_ITEMS_TEX_ID");
constexpr AssetID UNDERGROUND_ITEMS_TEX_ID = AssetID("UNDERGROUND_ITEMS_TEX_ID");
constexpr AssetID CASTLE_ITEMS_TEX_ID = AssetID("CASTLE_ITEMS_TEX_ID");
constexpr AssetID UNDERWATER_ITEMS_TEX_ID = AssetID("UNDERWATER_ITEMS_TEX_ID");

// Coin (T = 1) -> Walking/Spinning (A = 200)
constexpr AssetID COIN_SPIN_ANIM_ID = AssetID("COIN_SPIN_ANIM_ID");
constexpr AssetID COIN_SPIN_SPRITE_1 = AssetID("COIN_SPIN_SPRITE_1");
constexpr AssetID COIN_SPIN_SPRITE_2 = AssetID("COIN_SPIN_SPRITE_2");
constexpr AssetID COIN_SPIN_SPRITE_3 = AssetID("COIN_SPIN_SPRITE_3");
constexpr AssetID COIN_SPIN_SPRITE_4 = AssetID("COIN_SPIN_SPRITE_4");

// Mushroom (T = 2) -> Walking (A = 200)
constexpr AssetID MUSHROOM_ANIM_ID = AssetID("MUSHROOM_ANIM_ID");
constexpr AssetID MUSHROOM_SPRITE_1 = AssetID("MUSHROOM_SPRITE_1");

// Flower (T = 3) -> Idle (A = 100)
constexpr AssetID FLOWER_IDLE_ANIM_ID = AssetID("FLOWER_IDLE_ANIM_ID");
constexpr AssetID FLOWER_IDLE_SPRITE_1 = AssetID("FLOWER_IDLE_SPRITE_1");
constexpr AssetID FLOWER_IDLE_SPRITE_2 = AssetID("FLOWER_IDLE_SPRITE_2");
constexpr AssetID FLOWER_IDLE_SPRITE_3 = AssetID("FLOWER_IDLE_SPRITE_3");
constexpr AssetID FLOWER_IDLE_SPRITE_4 = AssetID("FLOWER_IDLE_SPRITE_4");

// Star (T = 4) -> Idle (A = 100)
constexpr AssetID STAR_IDLE_ANIM_ID = AssetID("STAR_IDLE_ANIM_ID");
constexpr AssetID STAR_IDLE_SPRITE_1 = AssetID("STAR_IDLE_SPRITE_1");
constexpr AssetID STAR_IDLE_SPRITE_2 = AssetID("STAR_IDLE_SPRITE_2");
constexpr AssetID STAR_IDLE_SPRITE_3 = AssetID("STAR_IDLE_SPRITE_3");
constexpr AssetID STAR_IDLE_SPRITE_4 = AssetID("STAR_IDLE_SPRITE_4");

// Flag (T = 5) -> Idle (A = 100)
constexpr AssetID FLAG_IDLE_ANIM_ID = AssetID("FLAG_IDLE_ANIM_ID");
constexpr AssetID FLAG_IDLE_SPRITE_1 = AssetID("FLAG_IDLE_SPRITE_1");

// Superleaf/ Raccoon suit (T = 6) -> Idle (A = 100)
constexpr AssetID SUPERLEAF_IDLE_ANIM_ID = AssetID("SUPERLEAF_IDLE_ANIM_ID");
constexpr AssetID SUPERLEAF_IDLE_SPRITE_1 = AssetID("SUPERLEAF_IDLE_SPRITE_1");
//Axe (T = 7) 
constexpr AssetID AXE_ANIM_ID = AssetID("AXE_ANIM_ID");
constexpr AssetID AXE_SPRITE_1 = AssetID("AXE_SPRITE_1");


// ==========================================
// HUD ELEMENTS (E = 5)
// ==========================================
constexpr AssetID HUDCOIN_TEX_ID = AssetID("HUDCOIN_TEX_ID");
constexpr AssetID HUDCOIN_SPRITE_ID = AssetID("HUDCOIN_SPRITE_ID");
constexpr AssetID HUDMARIO_SPRITE_ID = AssetID("HUDMARIO_SPRITE_ID");


// ==========================================
// FIREBALL (E = 6)
// ==========================================
constexpr AssetID FIREBALL_TEX_ID = AssetID("FIREBALL_TEX_ID");

// Walking/Bouncing (A = 200)
constexpr AssetID FIREBALL_BOUNCE_ANIM_ID = AssetID("FIREBALL_BOUNCE_ANIM_ID");
constexpr AssetID FIREBALL_BOUNCE_SPRITE_1 = AssetID("FIREBALL_BOUNCE_SPRITE_1");
constexpr AssetID FIREBALL_BOUNCE_SPRITE_2 = AssetID("FIREBALL_BOUNCE_SPRITE_2");
constexpr AssetID FIREBALL_BOUNCE_SPRITE_3 = AssetID("FIREBALL_BOUNCE_SPRITE_3");
constexpr AssetID FIREBALL_BOUNCE_SPRITE_4 = AssetID("FIREBALL_BOUNCE_SPRITE_4");

// Collision/Explode (A = 300)
constexpr AssetID FIREBALL_COLLIDE_ANIM_ID = AssetID("FIREBALL_COLLIDE_ANIM_ID");
constexpr AssetID FIREBALL_COLLIDE_SPRITE_1 = AssetID("FIREBALL_COLLIDE_SPRITE_1");
constexpr AssetID FIREBALL_COLLIDE_SPRITE_2 = AssetID("FIREBALL_COLLIDE_SPRITE_2");
constexpr AssetID FIREBALL_COLLIDE_SPRITE_3 = AssetID("FIREBALL_COLLIDE_SPRITE_3");

// Castle FLAG
constexpr AssetID CASTLE_FLAG_SPRITE_1 = AssetID("CASTLE_FLAG_SPRITE_1");



// ==========================================
// BOWSER (E = 8)
// ==========================================
constexpr AssetID BOWSER_TEX_ID = AssetID("BOWSER_TEX_ID"); // Fixed from 81000 to prevent internal overlap

// WALKING (A = 100)
constexpr AssetID BOWSER_WALK_ANIM_ID = AssetID("BOWSER_WALK_ANIM_ID");
constexpr AssetID BOWSER_WALK_SPRITE_1 = AssetID("BOWSER_WALK_SPRITE_1");
constexpr AssetID BOWSER_WALK_SPRITE_2 = AssetID("BOWSER_WALK_SPRITE_2");

// FIRE BREATHING (A = 300)
constexpr AssetID BOWSER_FIRE_BREATH_ANIM_ID = AssetID("BOWSER_FIRE_BREATH_ANIM_ID");
constexpr AssetID BOWSER_FIRE_BREATH_SPRITE_1 = AssetID("BOWSER_FIRE_BREATH_SPRITE_1");
constexpr AssetID BOWSER_FIRE_BREATH_SPRITE_2 = AssetID("BOWSER_FIRE_BREATH_SPRITE_2");

// HAMMER THROWING (A = 400)
constexpr AssetID BOWSER_HAMMER_THROW_ANIM_ID = AssetID("BOWSER_HAMMER_THROW_ANIM_ID");
constexpr AssetID BOWSER_HAMMER_THROW_SPRITE = AssetID("BOWSER_HAMMER_THROW_SPRITE");

//DYING (A = 500)
constexpr AssetID BOWSER_DEATH_ANIM_ID = AssetID("BOWSER_DEATH_ANIM_ID");
constexpr AssetID BOWSER_DEATH_SPRITE_1 = AssetID("BOWSER_DEATH_SPRITE_1");

// FALLING (A = 600)
constexpr AssetID BOWSER_FALL_ANIM_ID = AssetID("BOWSER_FALL_ANIM_ID");
constexpr AssetID BOWSER_FALL_SPRITE_1 = AssetID("BOWSER_FALL_SPRITE_1");
constexpr AssetID BOWSER_FALL_SPRITE_2 = AssetID("BOWSER_FALL_SPRITE_2");
constexpr AssetID BOWSER_FALL_SPRITE_3 = AssetID("BOWSER_FALL_SPRITE_3");


// ==========================================
// BOWSER ITEM BULLET (E = 9)  <-- FIXED from 100000 to prevent overlap with Points
// ==========================================
constexpr AssetID BOWSER_ITEM_BULLET_TEX_ID = AssetID("BOWSER_ITEM_BULLET_TEX_ID");

// Fire breath (T = 1) -> Walking (A = 200)
constexpr AssetID BOWSER_ITEM_BULLET_FIRE_ANIM_ID = AssetID("BOWSER_ITEM_BULLET_FIRE_ANIM_ID");
constexpr AssetID BOWSER_ITEM_BULLET_FIRE_SPRITE_1 = AssetID("BOWSER_ITEM_BULLET_FIRE_SPRITE_1");
constexpr AssetID BOWSER_ITEM_BULLET_FIRE_SPRITE_2 = AssetID("BOWSER_ITEM_BULLET_FIRE_SPRITE_2");

// Hammer (T = 2) -> Walking (A = 200)
constexpr AssetID BOWSER_ITEM_BULLET_HAMMER_ANIM_ID = AssetID("BOWSER_ITEM_BULLET_HAMMER_ANIM_ID");
constexpr AssetID BOWSER_ITEM_BULLET_HAMMER_SPRITE_1 = AssetID("BOWSER_ITEM_BULLET_HAMMER_SPRITE_1");
constexpr AssetID BOWSER_ITEM_BULLET_HAMMER_SPRITE_2 = AssetID("BOWSER_ITEM_BULLET_HAMMER_SPRITE_2");
constexpr AssetID BOWSER_ITEM_BULLET_HAMMER_SPRITE_3 = AssetID("BOWSER_ITEM_BULLET_HAMMER_SPRITE_3");
constexpr AssetID BOWSER_ITEM_BULLET_HAMMER_SPRITE_4 = AssetID("BOWSER_ITEM_BULLET_HAMMER_SPRITE_4");


// ==========================================
// POINT POPUPS (E = 10
// ==========================================
constexpr AssetID POINTS_TEX_ID = AssetID("POINTS_TEX_ID");

constexpr AssetID POINT_100_ANIM_ID = AssetID("POINT_100_ANIM_ID");
constexpr AssetID POINT_100_SPRITE_1 = AssetID("POINT_100_SPRITE_1");

constexpr AssetID POINT_200_ANIM_ID = AssetID("POINT_200_ANIM_ID");
constexpr AssetID POINT_200_SPRITE_1 = AssetID("POINT_200_SPRITE_1");

constexpr AssetID POINT_400_ANIM_ID = AssetID("POINT_400_ANIM_ID");
constexpr AssetID POINT_400_SPRITE_1 = AssetID("POINT_400_SPRITE_1");

constexpr AssetID POINT_500_ANIM_ID = AssetID("POINT_500_ANIM_ID");
constexpr AssetID POINT_500_SPRITE_1 = AssetID("POINT_500_SPRITE_1");

constexpr AssetID POINT_800_ANIM_ID = AssetID("POINT_800_ANIM_ID");
constexpr AssetID POINT_800_SPRITE_1 = AssetID("POINT_800_SPRITE_1");

constexpr AssetID POINT_1000_ANIM_ID = AssetID("POINT_1000_ANIM_ID");
constexpr AssetID POINT_1000_SPRITE_1 = AssetID("POINT_1000_SPRITE_1");

constexpr AssetID POINT_2000_ANIM_ID = AssetID("POINT_2000_ANIM_ID");
constexpr AssetID POINT_2000_SPRITE_1 = AssetID("POINT_2000_SPRITE_1");

constexpr AssetID POINT_4000_ANIM_ID = AssetID("POINT_4000_ANIM_ID");
constexpr AssetID POINT_4000_SPRITE_1 = AssetID("POINT_4000_SPRITE_1");

constexpr AssetID POINT_5000_ANIM_ID = AssetID("POINT_5000_ANIM_ID");
constexpr AssetID POINT_5000_SPRITE_1 = AssetID("POINT_5000_SPRITE_1");

constexpr AssetID POINT_8000_ANIM_ID = AssetID("POINT_8000_ANIM_ID");
constexpr AssetID POINT_8000_SPRITE_1 = AssetID("POINT_8000_SPRITE_1");

// 1-UP takes next entity slot natively in format (E = 11)
constexpr AssetID ONEUP_ANIM_ID = AssetID("ONEUP_ANIM_ID");
constexpr AssetID ONEUP_SPRITE_1 = AssetID("ONEUP_SPRITE_1");


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
    WORLD_CLEAR = -4,
    GROUND_THEME = -3,
    UNDERGROUND_THEME = -2,
    CASTLE_THEME = -1,
    UNDERWATER_THEME = 0,
    SMB3_WORLD1 = -5,


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
	TWIRL = 17,
    TAIL = 18,
    RACCOON_READY_TO_FLY = 19,
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