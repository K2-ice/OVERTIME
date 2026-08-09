#pragma once
#include <SFML/Graphics.hpp>
#include "InputManager.h"
#include "Player.h"
#include "MainMenuScreen.h"
#include "SettingsScreen.h"
#include "PauseScreen.h"
#include "SkillTreeScreen.h"
#include "GameOverScreen.h"

enum class GameState
{
    MainMenu,
    Playing,
    Paused,
    SkillTree,
    Settings,
    GameOver
};

class SceneManager
{
public:
    SceneManager(sf::Font& font);

    GameState getState() const;
    void setState(GameState newState);

    bool updateMenus(EngineL::InputManager& input, const sf::RenderWindow& window, EngineL::Player& player);
    void renderCurrentMenu(sf::RenderWindow& window, EngineL::InputManager& input, EngineL::Player& player, float survivedTime);

    SettingsScreen& getSettingsScreen();
    SkillTreeScreen& getSkillTreeScreen();
    MainMenuScreen& getMainMenuScreen();

    bool consumeContinueRequested();

private:
    GameState state = GameState::MainMenu;

    MainMenuScreen mainMenuScreen;
    SettingsScreen settingsScreen;
    PauseScreen pauseScreen;
    SkillTreeScreen skillTreeScreen;
    GameOverScreen gameOverScreen;

    bool continueRequested = false;
};