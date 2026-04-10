#pragma once
#include <queue>

#include "Scene.h"
#include <vector>

#include "GameObject.h"
#include "Mario.h"

using std::vector;

class PlayableScene :
    public Scene
{
    std::queue<GameObject*> addPendingGos;

    Mario* player;
    vector<GameObject*> objects;
    SceneContext* ctx;
public:
    explicit PlayableScene(const int level) : Scene(level)
    {
        player = nullptr; // init on load
        ctx = nullptr; // init on load
    }

    void Update(float dt) override;
    void Load() override;
    void UnLoad() override;
    void Render() override;
    void CleanupDeletedObjects();
    void AddObject(GameObject* go);
};

