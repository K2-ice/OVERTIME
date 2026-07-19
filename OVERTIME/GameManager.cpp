#include "GameManager.h"

GameManager::GameManager()
    : engine(1280, 720, "OVERTIME")
    , player(400.f, 300.f, &engine.getInputManager())
{
    updateManager.add(&player);
    renderManager.add(&player);
}

void GameManager::run()
{
    while (engine.isRunning())
    {
        float deltaTime = engine.beginFrame();

        sf::Vector2i mousePixel = engine.getInputManager().getMousePosition(engine.getWindow().getRenderWindow());
        player.aimAt(static_cast<float>(mousePixel.x), static_cast<float>(mousePixel.y));

        handleShooting();
        cleanupBullets();

        updateManager.updateAll(deltaTime);
        renderManager.renderAll(engine.getRenderer());

        engine.endFrame();
    }
}

void GameManager::handleShooting()
{
    if (player.wantsToShoot())
    {
        float x = player.getPosition().x;
        float y = player.getPosition().y;
        float dirX = player.getShootDirectionX();
        float dirY = player.getShootDirectionY();

        EngineL::Bullet* bullet = new EngineL::Bullet(x, y, dirX, dirY);

        bullets.push_back(bullet);
        updateManager.add(bullet);
        renderManager.add(bullet);

        player.resetShootCooldown();
    }
}

void GameManager::cleanupBullets()
{
    for (int i = 0; i < bullets.size(); i++)
    {
        EngineL::Bullet* bullet = bullets[i];

        float x = bullet->getPosition().x;
        float y = bullet->getPosition().y;

        bool outOfScreen = x < 0.f || x > 1280.f || y < 0.f || y > 720.f;

        if (outOfScreen)
        {
            updateManager.remove(bullet);
            renderManager.remove(bullet);

            delete bullet;
            bullets.erase(bullets.begin() + i);
            i--;
        }
    }
}