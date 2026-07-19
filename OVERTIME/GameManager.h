#pragma once
#include "GameEngine.h"
#include "Player.h"
#include "Bullet.h"
#include "UpdateManager.h"
#include "RenderManager.h"
#include <vector>

class GameManager
{
public:
    GameManager();

    void run();

private:
    void handleShooting();
    void cleanupBullets();

    EngineL::GameEngine engine;
    EngineL::Player player;
    EngineL::UpdateManager updateManager;
    EngineL::RenderManager renderManager;

    std::vector<EngineL::Bullet*> bullets;
};