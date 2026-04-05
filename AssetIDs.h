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


/// MARIO (E = 10000, T = 1)
constexpr int MARIO_TEX_ID = 11000;

// Idle (A = 100)
constexpr int MARIO_IDLE_ANIM_ID = 11100;
constexpr int MARIO_IDLE_SPRITE_1 = 11101;


// Running (A = 200)
constexpr int MARIO_RUN_ANIM_ID = 11200;

constexpr int MARIO_RUN_SPRITE_1 = 11201;
constexpr int MARIO_RUN_SPRITE_2 = 11202;
constexpr int MARIO_RUN_SPRITE_3 = 11203;

/// ================================

/// GOOMBA (E = 20000, T = 1)
constexpr int GOOMBA_TEX_ID = 21000;

// Walking (A = 100)
constexpr int GOOMBA_WALK_ANIM_ID = 21100;
constexpr int GOOMBA_WALK_SPRITE_1 = 21101;
constexpr int GOOMBA_WALK_SPRITE_2 = 21102;

// Dead (A = 200)
constexpr int GOOMBA_DEAD_ANIM_ID = 21200;
constexpr int GOOMBA_DEAD_SPRITE_1 = 21201;
/// ==================
