#pragma once
#include "LevelLoader.h"
#include "Scene.h"
#include <vector>

#include "GameObject.h"
#include "Mario.h"

using std::vector;

class PlayableScene :
    public Scene
{
    Mario* player;
    vector<GameObject*> objects;
    SceneContext* ctx;
public:

    void Update(float dt) override;
    void Load() override;
    void UnLoad() override;
    void Render() override;
    void CleanupDeletedObjects();
};

