#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include "Player.h"
#include "Bullet.h"
#include "UpdateManager.h"
#include "RenderManager.h"
#include "CollisionManager.h"

namespace EngineL
{
    class EnemyManager
    {
    public:
        EnemyManager(UpdateManager& updateManager, RenderManager& renderManager, Player* player);

        std::vector<Enemy*>& getEnemies();

        void update(float deltaTime);

        void checkBulletCollisions(std::vector<Bullet*>& bullets);
        void checkPlayerCollision();

        std::vector<sf::Vector2f> removeDeadEnemies();

        void clear();

    private:
        void spawnEnemy();
        void pushEnemyAwayFromPlayer(Enemy* enemy);

        UpdateManager& updateManager;
        RenderManager& renderManager;
        Player* player;

        std::vector<Enemy*> enemies;

        float spawnTimer = 0.f;
        float spawnDelay = 1.f;
    };
}