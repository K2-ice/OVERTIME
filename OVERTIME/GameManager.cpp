#include "pch.h"
#include "GameManager.h"

#include <cmath>
#include <cstdlib>

GameManager::GameManager()
    : engine(1280, 720, "OVERTIME")
    , player(400.f, 300.f, &engine.getInputManager())
    , enemyManager(updateManager, renderManager, &player)
    , hud(font)
    , skillTreeScreen(font)
    , mainMenuScreen(font)
    , settingsScreen(font)
    , pauseScreen(font)
    , gameOverScreen(font)
{
    updateManager.add(&player);
    renderManager.add(&player);

    weaponInventory.reset(player.hasSecondWeaponSlot());

    if (!font.openFromFile("Assets/arial.ttf"))
        throw std::runtime_error("Failed to load font.");

    player.addSouls(50);
}

GameManager::~GameManager()
{
    for (auto bullet : bullets)
    {
        updateManager.remove(bullet);
        renderManager.remove(bullet);
        delete bullet;
    }

    for (auto soul : souls)
    {
        updateManager.remove(soul);
        renderManager.remove(soul);
        delete soul;
    }

    for (auto pickup : weaponPickups)
    {
        updateManager.remove(pickup);
        renderManager.remove(pickup);
        delete pickup;
    }

    enemyManager.clear();
}

void GameManager::run()
{
    while (engine.isRunning())
    {
        float deltaTime = engine.beginFrame();

        update(deltaTime);
        render();

        engine.endFrame();
    }
}

void GameManager::update(float deltaTime)
{
    if (state == GameState::MainMenu)
    {
        MainMenuAction action = mainMenuScreen.handleInput(
            engine.getInputManager(),
            engine.getWindow().getRenderWindow());

        if (action == MainMenuAction::Play)
        {
            startNewRun();
            state = GameState::Playing;
        }
        else if (action == MainMenuAction::Settings)
        {
            state = GameState::Settings;
        }

        return;
    }

    if (state == GameState::Settings)
    {
        SettingsAction action = settingsScreen.handleInput(
            engine.getInputManager(),
            engine.getWindow().getRenderWindow());

        if (action == SettingsAction::Back)
        {
            state = GameState::MainMenu;
        }

        return;
    }

    if (state == GameState::SkillTree)
    {
        skillTreeScreen.handleInput(
            engine.getInputManager(),
            engine.getWindow().getRenderWindow(),
            player);

        if (engine.getInputManager().isStartPressed())
        {
            startNewRun();
            state = GameState::Playing;
        }

        return;
    }

    if (state == GameState::Paused)
    {
        PauseAction action = pauseScreen.handleInput(
            engine.getInputManager(),
            engine.getWindow().getRenderWindow());

        if (action == PauseAction::Resume)
        {
            state = GameState::Playing;
        }
        else if (action == PauseAction::QuitToMenu)
        {
            state = GameState::MainMenu;
        }

        return;
    }

    if (state == GameState::GameOver)
    {
        GameOverAction action = gameOverScreen.handleInput(
            engine.getInputManager(),
            engine.getWindow().getRenderWindow());

        if (action == GameOverAction::Retry)
        {
            startNewRun();
            state = GameState::Playing;
        }
        else if (action == GameOverAction::QuitToMenu)
        {
            state = GameState::MainMenu;
        }

        return;
    }

    // etat "Playing"

    if (engine.getInputManager().isPausePressed())
    {
        state = GameState::Paused;
        return;
    }

    sf::Vector2i mouse =
        engine.getInputManager().getMousePosition(
            engine.getWindow().getRenderWindow());

    player.aimAt(
        static_cast<float>(mouse.x),
        static_cast<float>(mouse.y));

    runTime += deltaTime;

    if (runTime >= maxRunTime)
    {
        state = GameState::SkillTree;
        return;
    }

    enemyManager.update(deltaTime);

    weaponInventory.handleSwitch(engine.getInputManager(), player.hasSecondWeaponSlot());
    handleReload();

    weaponInventory.getCurrentWeapon()->update(deltaTime);
    handleShooting();

    updateManager.updateAll(deltaTime);

    enemyManager.checkBulletCollisions(bullets);
    enemyManager.checkPlayerCollision();

    handleEnemyDeaths();
    collectSouls();
    collectWeaponPickups();
    cleanupBullets();

    if (player.GetStats().health <= 0)
    {
        state = GameState::GameOver;
    }
}

void GameManager::render()
{
    if (state == GameState::MainMenu)
    {
        mainMenuScreen.render(engine.getWindow().getRenderWindow());
    }
    else if (state == GameState::Settings)
    {
        settingsScreen.render(engine.getWindow().getRenderWindow());
    }
    else if (state == GameState::SkillTree)
    {
        skillTreeScreen.render(engine.getWindow().getRenderWindow(), player);
    }
    else if (state == GameState::Paused)
    {
        renderGameScene();
        pauseScreen.render(engine.getWindow().getRenderWindow());
    }
    else if (state == GameState::GameOver)
    {
        gameOverScreen.render(engine.getWindow().getRenderWindow(), runTime);
    }
    else
    {
        renderGameScene();
    }
}

void GameManager::handleReload()
{
    if (engine.getInputManager().isReloadPressed())
    {
        weaponInventory.getCurrentWeapon()->startReload();
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

        std::vector<EngineL::Bullet*> newBullets =
            weaponInventory.getCurrentWeapon()->fire(x, y, dirX, dirY);

        for (EngineL::Bullet* bullet : newBullets)
        {
            bullets.push_back(bullet);
            updateManager.add(bullet);
            renderManager.add(bullet);
        }
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

void GameManager::handleEnemyDeaths()
{
    std::vector<sf::Vector2f> deathPositions = enemyManager.removeDeadEnemies();

    for (const sf::Vector2f& position : deathPositions)
    {
        EngineL::Soul* soul = new EngineL::Soul(position.x, position.y);

        souls.push_back(soul);
        updateManager.add(soul);
        renderManager.add(soul);

        EngineL::WeaponPickup* pickup = weaponInventory.tryDropWeapon(position.x, position.y);

        if (pickup != nullptr)
        {
            weaponPickups.push_back(pickup);
            updateManager.add(pickup);
            renderManager.add(pickup);
        }
    }
}

void GameManager::collectSouls()
{
    for (int i = 0; i < souls.size(); i++)
    {
        EngineL::Soul* soul = souls[i];

        if (EngineL::CollisionManager::checkCollision(&player, soul))
        {
            player.addSouls(soul->getValue());

            updateManager.remove(soul);
            renderManager.remove(soul);

            delete soul;

            souls.erase(souls.begin() + i);
            i--;
        }
    }
}

void GameManager::collectWeaponPickups()
{
    for (int i = 0; i < weaponPickups.size(); i++)
    {
        EngineL::WeaponPickup* pickup = weaponPickups[i];

        if (EngineL::CollisionManager::checkCollision(&player, pickup))
        {
            EngineL::Weapon* weapon = weaponInventory.getWeaponById(pickup->getWeaponId());

            if (weapon != nullptr && !weaponInventory.isInInventory(weapon))
            {
                weaponInventory.equipPickup(weapon);
            }

            updateManager.remove(pickup);
            renderManager.remove(pickup);

            delete pickup;

            weaponPickups.erase(weaponPickups.begin() + i);
            i--;
        }
    }
}

void GameManager::startNewRun()
{
    runTime = 0.f;

    player.setPosition(400.f, 300.f);

    Stats& stats = player.GetStats();
    stats.health = stats.maxHealth;

    weaponInventory.reset(player.hasSecondWeaponSlot());

    for (auto bullet : bullets)
    {
        updateManager.remove(bullet);
        renderManager.remove(bullet);
        delete bullet;
    }
    bullets.clear();

    for (auto soul : souls)
    {
        updateManager.remove(soul);
        renderManager.remove(soul);
        delete soul;
    }
    souls.clear();

    for (auto pickup : weaponPickups)
    {
        updateManager.remove(pickup);
        renderManager.remove(pickup);
        delete pickup;
    }
    weaponPickups.clear();

    enemyManager.clear();
}

void GameManager::renderGameScene()
{
    renderManager.renderAll(engine.getRenderer());

    hud.draw(
        engine.getWindow().getRenderWindow(),
        player,
        weaponInventory.getCurrentWeapon(),
        runTime,
        maxRunTime,
        player.hasSecondWeaponSlot());
}