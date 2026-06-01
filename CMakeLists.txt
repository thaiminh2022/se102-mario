cmake_minimum_required(VERSION 3.21)

project(se102_mario LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DXSDK_D3DX_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/packages/Microsoft.DXSDK.D3DX.9.29.952.8/build/native")

set(SOURCES
    "Source/Scenes/GameOverScene.cpp"
    "Source/Scenes/GameOverScene.h"
    Source/App/main.cpp
    Source/Collision/Collision.cpp
    Source/Collision/Collision.h
    Source/Collision/CollisionEvent.h
    Source/Core/AssetIDs.h
    Source/Core/Camera.cpp
    Source/Core/Camera.h
    Source/Core/Color.h
    Source/Core/Debug.cpp
    Source/Core/Debug.h
    Source/Core/Event.h
    Source/Core/Game.cpp
    Source/Core/Game.h
    Source/Core/GameObject.h
    Source/Core/Helper.h
    Source/Core/OptionalType.h
    Source/Core/Rect.h
    Source/Core/Scene.h
    Source/Core/SceneSwitchContext.h
    Source/Core/Timer.cpp
    Source/Core/Timer.h
    Source/Core/Vector2.h
    Source/Entities/Enemies/Bloopers.cpp
    Source/Entities/Enemies/Bloopers.h
    Source/Entities/Enemies/Bowser.cpp
    Source/Entities/Enemies/Bowser.h
    Source/Entities/Enemies/CheepCheeps.cpp
    Source/Entities/Enemies/CheepCheeps.h
    Source/Entities/Enemies/Goomba.cpp
    Source/Entities/Enemies/Goomba.h
    Source/Entities/Enemies/Koopa.cpp
    Source/Entities/Enemies/Koopa.h
    Source/Entities/Items/BrickExplode.cpp
    Source/Entities/Items/BrickExplode.h
    Source/Entities/Items/Coin.cpp
    Source/Entities/Items/Coin.h
    Source/Entities/Items/Fireball.cpp
    Source/Entities/Items/Fireball.h
    Source/Entities/Items/Flower.cpp
    Source/Entities/Items/Flower.h
    Source/Entities/Items/Mushroom.cpp
    Source/Entities/Items/Mushroom.h
    Source/Entities/Items/PointPopup.cpp
    Source/Entities/Items/PointPopup.h
    Source/Entities/Items/QuestionBlock.cpp
    Source/Entities/Items/QuestionBlock.h
    Source/Entities/Items/Star.cpp
    Source/Entities/Items/Star.h
    Source/Entities/Mario/Mario.cpp
    Source/Entities/Mario/Mario.h
    Source/Entities/Mario/MarioBreathBubble.cpp
    Source/Entities/Mario/MarioBreathBubble.h
    Source/Entities/Mario/MarioCollision.cpp
    Source/Entities/Mario/MarioInput.cpp
    Source/Entities/Mario/MarioJetPack.cpp
    Source/Entities/Mario/MarioJetPack.h
    Source/Entities/Mario/MarioLoader.cpp
    Source/Entities/Mario/MarioRendering.cpp
    Source/Entities/Mario/MarioSpecialStates.cpp
    Source/Entities/Projectiles/BowserFireBullet.cpp
    Source/Entities/Projectiles/BowserFireBullet.h
    Source/Entities/Projectiles/BowserHammer.cpp
    Source/Entities/Projectiles/BowserHammer.h
    Source/Entities/Triggers/BgMusicTrigger.cpp
    Source/Entities/Triggers/BgMusicTrigger.h
    Source/Entities/Triggers/ClearScreenColorTrigger.cpp
    Source/Entities/Triggers/ClearScreenColorTrigger.h
    Source/Entities/Triggers/EnterCastleTrigger.cpp
    Source/Entities/Triggers/EnterCastleTrigger.h
    Source/Entities/Triggers/FireballTrap.cpp
    Source/Entities/Triggers/FireballTrap.h
    Source/Entities/Triggers/FireShooter.cpp
    Source/Entities/Triggers/FireShooter.h
    Source/Entities/Triggers/FlagPole.cpp
    Source/Entities/Triggers/FlagPole.h
    Source/Entities/Triggers/InWaterTrigger.cpp
    Source/Entities/Triggers/InWaterTrigger.h
    Source/Entities/Triggers/LevelTextRender.cpp
    Source/Entities/Triggers/LevelTextRender.h
    Source/Entities/Triggers/NextLevelPortal.cpp
    Source/Entities/Triggers/NextLevelPortal.h
    Source/Entities/Triggers/Pipe.cpp
    Source/Entities/Triggers/Pipe.h
    Source/Managers/AudioManager.cpp
    Source/Managers/AudioManager.h
    Source/Managers/InputManager.cpp
    Source/Managers/InputManager.h
    Source/Managers/StatManager.cpp
    Source/Managers/StatManager.h
    Source/Rendering/Animation.cpp
    Source/Rendering/Animation.h
    Source/Rendering/Animations.cpp
    Source/Rendering/Animations.h
    Source/Rendering/FontManager.cpp
    Source/Rendering/FontManager.h
    Source/Rendering/Sprite.cpp
    Source/Rendering/Sprite.h
    Source/Rendering/Sprites.cpp
    Source/Rendering/Sprites.h
    Source/Rendering/Texture.h
    Source/Rendering/Textures.cpp
    Source/Rendering/Textures.h
    Source/Scenes/LevelTransitionScene.cpp
    Source/Scenes/LevelTransitionScene.h
    Source/Scenes/MainMenu.cpp
    Source/Scenes/MainMenu.h
    Source/Scenes/PlayableScene.cpp
    Source/Scenes/PlayableScene.h
    Source/UI/HUD.cpp
    Source/UI/HUD.h
    Source/UI/UIElement.cpp
    Source/UI/UIElement.h
    Source/UI/UILabel.cpp
    Source/UI/UILabel.h
    Source/World/Bridge.cpp
    Source/World/Bridge.h
    Source/World/CollisionMatrix.cpp
    Source/World/CollisionMatrix.h
    Source/World/LdtkParser.h
    Source/World/LevelLoader.cpp
    Source/World/LevelLoader.h
    Source/World/SceneEntityData.h
    Source/World/Tile.h
    Source/World/Tilemap.cpp
    Source/World/Tilemap.h
    Source/World/TilemapConfig.h
        Source/Entities/Triggers/ForceVelocityTrigger.cpp
        Source/Entities/Triggers/ForceVelocityTrigger.h
)

add_executable(se102-mario WIN32 ${SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

target_compile_definitions(se102-mario PRIVATE
    _WINDOWS
    UNICODE
    _UNICODE
    HAS_DXSDK_D3DX
)

target_include_directories(se102-mario PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/App"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Collision"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Core"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Entities"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Entities/Enemies"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Entities/Items"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Entities/Mario"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Entities/Projectiles"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Entities/Triggers"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Managers"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Rendering"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Scenes"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/UI"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/World"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor"
    "${DXSDK_D3DX_ROOT}/include"
)

target_link_directories(se102-mario PRIVATE
    "$<$<CONFIG:Debug>:${DXSDK_D3DX_ROOT}/debug/lib/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>>"
    "$<$<NOT:$<CONFIG:Debug>>:${DXSDK_D3DX_ROOT}/release/lib/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>>"
)

target_link_libraries(se102-mario PRIVATE
    d3d10
    DXGI
    "$<$<CONFIG:Debug>:d3dx9d>"
    "$<$<CONFIG:Debug>:d3dx10d>"
    "$<$<CONFIG:Debug>:d3dx11d>"
    "$<$<NOT:$<CONFIG:Debug>>:d3dx9>"
    "$<$<NOT:$<CONFIG:Debug>>:d3dx10>"
    "$<$<NOT:$<CONFIG:Debug>>:d3dx11>"
)

set_target_properties(se102-mario PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_custom_command(TARGET se102-mario POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "$<IF:$<CONFIG:Debug>,${DXSDK_D3DX_ROOT}/debug/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>,${DXSDK_D3DX_ROOT}/release/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>>"
        "$<TARGET_FILE_DIR:se102-mario>"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Assets"
        "$<TARGET_FILE_DIR:se102-mario>/Assets"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/world_map-new.ldtk"
        "${CMAKE_CURRENT_SOURCE_DIR}/world_map-og.ldtk"
        "$<TARGET_FILE_DIR:se102-mario>"
)
