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

// Turning Around (A = 300)
constexpr int MARIO_TURN_ANIM_ID = 11300;
constexpr int MARIO_TURN_SPRITE_1 = 11301;


// Jumping (A = 400)
constexpr int MARIO_JUMP_ANIM_ID = 11400;
constexpr int MARIO_JUMP_SPRITE_1 = 11401;

// Dying (A = 500)
constexpr int MARIO_DEATH_ANIM_ID = 11500;
constexpr int MARIO_DEATH_SPRITE_1 = 11501;


// Big Mario (E = 10000, T = 2)
constexpr int MARIO_BIG_TEX_ID = 12000;

// Idle (A = 100)
constexpr int MARIO_BIG_IDLE_ANIM_ID = 12100;
constexpr int MARIO_BIG_IDLE_SPRITE_1 = 12101;
// Running (A = 200)
constexpr int MARIO_BIG_RUN_ANIM_ID = 12200;
constexpr int MARIO_BIG_RUN_SPRITE_1 = 12201;
constexpr int MARIO_BIG_RUN_SPRITE_2 = 12202;
constexpr int MARIO_BIG_RUN_SPRITE_3 = 12203;	
// Turning Around (A = 300)
constexpr int MARIO_BIG_TURN_ANIM_ID = 12300;
constexpr int MARIO_BIG_TURN_SPRITE_1 = 12301;

// Jumping (A = 400)
constexpr int MARIO_BIG_JUMP_ANIM_ID = 12400;
constexpr int MARIO_BIG_JUMP_SPRITE_1 = 12401;

// Ducking (A = 500)
constexpr int MARIO_BIG_DUCK_ANIM_ID = 12500;
constexpr int MARIO_BIG_DUCK_SPRITE_1 = 12501;

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



/// MUSICS: Background music will be managed by negative values in ldtk
constexpr int MARIO_JUMP_SMALL = 1;
constexpr int MARIO_JUMP_BIG = 2;
constexpr int GOOMBA_STOMP = 3;
constexpr int MARIO_DIE = 4;



/// FONT
constexpr int STATS_FONT = 1;
