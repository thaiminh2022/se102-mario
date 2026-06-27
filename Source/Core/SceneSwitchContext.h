#pragma once
#include "OptionalType.h"
#include "Vector2.h"
#include "Rect.h"


enum class MarioPower;

struct MarioPipeCtx
{
    Vector2Int dir{};
    Rect returnZone{};
    Vector2Int moveTo{};
};

struct SceneSwitchContext
{
    bool useTransitionScene = false;
    Optional<MarioPipeCtx> marioCtx = {};
    MarioPower marioPower = (MarioPower)0;

    static SceneSwitchContext NormalTransition(const MarioPower power = (MarioPower)0)
    {
        SceneSwitchContext ctx;
        ctx.useTransitionScene = true;
		if (power != (MarioPower)3 && power != (MarioPower)4 && power != (MarioPower)5)
            ctx.marioPower = power;
        else 
			ctx.marioPower = (MarioPower)0;
        return ctx;
    }

    static SceneSwitchContext NoTransition(const MarioPower power = (MarioPower)0)
    {
        SceneSwitchContext ctx;
        ctx.marioPower = power;
        return ctx;
    }


    static SceneSwitchContext PipeTransition(
        const MarioPipeCtx& pipeCtx,
        const MarioPower power = (MarioPower)0
    )
    {
        SceneSwitchContext ctx;
        ctx.marioCtx = pipeCtx;
        ctx.marioPower = power;
        return ctx;
    }
};
