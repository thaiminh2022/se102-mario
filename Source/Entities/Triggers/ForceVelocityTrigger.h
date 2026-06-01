#pragma once
#include "GameObject.h"

class ForceVelocityTrigger : public GameObject{
    ForceVelocityTriggerData data;
    bool triggered = false;
public:
    explicit ForceVelocityTrigger(ForceVelocityTriggerData data);
    void Update(float dt, vector<GameObject *> &coObjects, SceneContext *ctx) override;
    Rect GetBoundingBox() override;
    Vector2Int GetMoveTo();
	bool IsCollidable() override { return false; }
    bool IsBlocking() override {return false;}
};


