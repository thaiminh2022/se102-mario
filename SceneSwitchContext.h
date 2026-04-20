#pragma once
#include "OptionalType.h"
#include "Vector2.h"

enum class SceneEnterType
{
	None,
	MarioFromPipe,
};

struct SceneSwitchContext
{
	bool useTransitionScene;
	Optional<Vector2Int> preferSpawnPoint;
	SceneEnterType sceneEnterType;

	SceneSwitchContext(const bool useTransitionScene, const Optional<Vector2Int>& preferSpawnPoint, SceneEnterType sceneEnterType)
		: useTransitionScene(useTransitionScene),
		  preferSpawnPoint(preferSpawnPoint),
		  sceneEnterType(sceneEnterType)
	{
	}
	SceneSwitchContext()
	{
		useTransitionScene = false;
		preferSpawnPoint = {};
		sceneEnterType = SceneEnterType::None;
	}

	static SceneSwitchContext UseTransitionScene()
	{
		return SceneSwitchContext(true, {}, SceneEnterType::None);
	}
};
