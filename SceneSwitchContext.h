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
    Optional<float> levelTimeLeft = {};
    MarioPower marioPower = (MarioPower)0;

    static SceneSwitchContext NormalTransition(const MarioPower power = (MarioPower)0)
    {
        SceneSwitchContext ctx;
        ctx.useTransitionScene = true;
        ctx.marioPower = power;
        return ctx;
    }

    static SceneSwitchContext NormalTransition(const MarioPower power, const float timeLeft)
    {
        SceneSwitchContext ctx = NormalTransition(power);
        ctx.levelTimeLeft.Set(timeLeft);
        return ctx;
    }

    static SceneSwitchContext NoTransition(const MarioPower power = (MarioPower)0)
    {
        SceneSwitchContext ctx;
        ctx.marioPower = power;
        return ctx;
    }

    static SceneSwitchContext NoTransition(const MarioPower power, const Optional<float>& timeLeft)
    {
        SceneSwitchContext ctx = NoTransition(power);
        ctx.levelTimeLeft = timeLeft;
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

    static SceneSwitchContext PipeTransition(
        const MarioPipeCtx& pipeCtx,
        const MarioPower power,
        const float timeLeft
    )
    {
        SceneSwitchContext ctx = PipeTransition(pipeCtx, power);
        ctx.levelTimeLeft.Set(timeLeft);
        return ctx;
    }
};
