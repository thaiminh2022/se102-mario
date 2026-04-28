#pragma once
#include "OptionalType.h"
#include "Vector2.h"
#include "Rect.h"

struct MarioPipeCtx
{
	Vector2Int dir;
	Rect returnZone;
	Vector2Int moveTo;
};

struct SceneSwitchContext
{
	bool useTransitionScene;
	Optional<MarioPipeCtx> marioCtx;

	static SceneSwitchContext UseTransitionScene()
	{
		return { true, {}};
	}
	static SceneSwitchContext WithMarioPipeExit(const MarioPipeCtx& ctx)
	{
		return { false, Optional<MarioPipeCtx>(ctx) };
	}
};
