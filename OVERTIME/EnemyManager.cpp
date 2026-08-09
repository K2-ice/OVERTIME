#include "pch.h"
#include "EnemyManager.h"

#include <cmath>
#include <cstdlib>

namespace EngineL
{
    EnemyManager::EnemyManager(UpdateManager& updateManager, RenderManager& renderManager, Player* player)
        : updateManager(updateManager)
        , renderManager(renderManager)
        , player(player)
    {
    }

    std::vector<Enemy*>& EnemyManager::getEnemies()
    {
        return enemies;
    }

    void EnemyManager::update(float deltaTime, float runTime)
    {
        spawnTimer += deltaTime;

        if (spawnTimer >= spawnDelay)
        {
            spawnTimer = 0.f;
            spawnEnemy();
        }

        if (!bossSpawned && runTime >= bossSpawnTime)
        {
            spawnBoss();
        }

        updateBossAttacks();
    }

    void EnemyManager::spawnEnemy()
    {
        sf::Vector2f playerPosition = player->getPosition();

        float angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 6.2831853f;

        float x = playerPosition.x + std::cos(angle) * 500.f;
        float y = playerPosition.y + std::sin(angle) * 500.f;

        int roll = rand() % 100;

        Enemy* enemy = nullptr;

        if (roll < 45)
        {
            enemy = new Enemy(x, y, player);           // Basique : 45%
        }
        else if (roll < 65)
        {
            enemy = new KamikazeEnemy(x, y, player);    // Kamikaze : 20%
        }
        else if (roll < 85)
        {
            enemy = new MeleeEnemy(x, y, player);       // Corps a corps : 20%
        }
        else
        {
            enemy = new BelierEnemy(x, y, player);         // Belier : 15%
        }

        enemies.push_back(enemy);

        updateManager.add(enemy);
        renderManager.add(enemy);
    }

    void EnemyManager::spawnBoss()
    {
        bossSpawned = true;

        sf::Vector2f playerPosition = player->getPosition();

        float angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 6.2831853f;

        float x = playerPosition.x + std::cos(angle) * 500.f;
        float y = playerPosition.y + std::sin(angle) * 500.f;

        boss = new Boss(x, y, player);

        enemies.push_back(boss);

        updateManager.add(boss);
        renderManager.add(boss);
    }

    void EnemyManager::updateBossAttacks()
    {
        if (boss == nullptr || !boss->isAlive())
            return;

        if (boss->wantsToFire())
        {
            std::vector<Bullet*> newBullets = boss->fire();

            for (Bullet* bullet : newBullets)
            {
                enemyBullets.push_back(bullet);
                updateManager.add(bullet);
                renderManager.add(bullet);
            }
        }
    }

    void EnemyManager::checkBulletCollisions(std::vector<Bullet*>& bullets)
    {
        for (int b = 0; b < bullets.size(); b++)
        {
            Bullet* bullet = bullets[b];

            bool bulletDestroyed = false;

            for (int e = 0; e < enemies.size(); e++)
            {
                Enemy* enemy = enemies[e];

                if (CollisionManager::checkCollision(bullet, enemy))
                {
                    enemy->takeDamage(static_cast<int>(bullet->getDamage()));

                    updateManager.remove(bullet);
                    renderManager.remove(bullet);

                    delete bullet;

                    bullets.erase(bullets.begin() + b);
                    b--;

                    bulletDestroyed = true;
                    break;
                }
            }

            if (bulletDestroyed)
                continue;
        }
    }

    void EnemyManager::checkEnemyBulletCollisions()
    {
        for (int b = 0; b < enemyBullets.size(); b++)
        {
            Bullet* bullet = enemyBullets[b];

            if (CollisionManager::checkCollision(bullet, player))
            {
                if (player->canTakeDamage())
                {
                    player->takeDamage(static_cast<int>(bullet->getDamage()));
                    player->resetDamageCooldown();
                }

                updateManager.remove(bullet);
                renderManager.remove(bullet);

                delete bullet;

                enemyBullets.erase(enemyBullets.begin() + b);
                b--;
            }
        }
    }

    void EnemyManager::checkPlayerCollision()
    {
        for (Enemy* enemy : enemies)
        {
            if (CollisionManager::checkCollision(enemy, player))
            {
                if (player->canTakeDamage())
                {
                    player->takeDamage(static_cast<int>(enemy->GetStats().damage));
                    player->resetDamageCooldown();
                }

                if (enemy->explodesOnContact())
                {
                    enemy->takeDamage(9999);
                }
                else
                {
                    pushEnemyAwayFromPlayer(enemy);
                }
            }
        }
    }

    void EnemyManager::pushEnemyAwayFromPlayer(Enemy* enemy)
    {
        sf::Vector2f enemyPos = enemy->getPosition();
        sf::Vector2f playerPos = player->getPosition();

        float dx = enemyPos.x - playerPos.x;
        float dy = enemyPos.y - playerPos.y;

        float length = std::sqrt(dx * dx + dy * dy);

        if (length == 0.f)
        {
            dx = 1.f;
            dy = 0.f;
            length = 1.f;
        }

        dx /= length;
        dy /= length;

        float pushDistance = 3.f;

        enemy->setPosition(enemyPos.x + dx * pushDistance, enemyPos.y + dy * pushDistance);
    }

    std::vector<sf::Vector2f> EnemyManager::removeDeadEnemies()
    {
        std::vector<sf::Vector2f> deathPositions;

        for (int i = 0; i < enemies.size(); i++)
        {
            Enemy* enemy = enemies[i];

            if (!enemy->isAlive())
            {
                deathPositions.push_back(enemy->getPosition());

                if (enemy == boss)
                    boss = nullptr;

                updateManager.remove(enemy);
                renderManager.remove(enemy);

                delete enemy;

                enemies.erase(enemies.begin() + i);
                i--;
            }
        }

        return deathPositions;
    }

    void EnemyManager::clear()
    {
        for (Enemy* enemy : enemies)
        {
            updateManager.remove(enemy);
            renderManager.remove(enemy);
            delete enemy;
        }

        enemies.clear();

        for (Bullet* bullet : enemyBullets)
        {
            updateManager.remove(bullet);
            renderManager.remove(bullet);
            delete bullet;
        }

        enemyBullets.clear();

        boss = nullptr;
        bossSpawned = false;
    }
}