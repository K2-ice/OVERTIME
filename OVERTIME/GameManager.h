#pragma once
#include <vector>
#include <string>
#include "GameEngine.h"
#include "Player.h"
#include "Bullet.h"
#include "Soul.h"
#include "WeaponPickup.h"
#include "WeaponInventory.h"
#include "CollisionManager.h"
#include "EnemyManager.h"
#include "UpdateManager.h"
#include "RenderManager.h"
#include "SkillTreeScreen.h"
#include "MainMenuScreen.h"
#include "SettingsScreen.h"
#include "PauseScreen.h"
#include "GameOverScreen.h"
#include "HUD.h"
#include <SFML/Graphics/Font.hpp>

enum class GameState
{
    MainMenu,
    Playing,
    Paused,
    SkillTree,
    Settings,
    GameOver
};

class GameManager
{
public:
    GameManager();
    ~GameManager();
    void run();

private:
    void update(float deltaTime);
    void render();
    void handleShooting();
    void handleReload();
    void cleanupBullets();
    void handleEnemyDeaths();
    void startNewRun();
    void renderGameScene();
    void collectSouls();
    void collectWeaponPickups();

    EngineL::GameEngine engine;
    EngineL::Player player;
    EngineL::UpdateManager updateManager;
    EngineL::RenderManager renderManager;

    EngineL::EnemyManager enemyManager;

    std::vector<EngineL::Bullet*> bullets;
    std::vector<EngineL::Soul*> souls;
    std::vector<EngineL::WeaponPickup*> weaponPickups;

    WeaponInventory weaponInventory;

    GameState state = GameState::MainMenu;

    float runTime = 0.f;
    float maxRunTime = 10.f;

    sf::Font font;
    HUD hud;
    SkillTreeScreen skillTreeScreen;
    MainMenuScreen mainMenuScreen;
    SettingsScreen settingsScreen;
    PauseScreen pauseScreen;
    GameOverScreen gameOverScreen;
};