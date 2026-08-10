#include "pch.h"
#include "SceneManager.h"

SceneManager::SceneManager(sf::Font& font)
	: mainMenuScreen(font)
	, settingsScreen(font)
	, pauseScreen(font)
	, skillTreeScreen(font)
	, gameOverScreen(font)
{
}

GameState SceneManager::getState() const
{
	return state;
}

void SceneManager::setState(GameState newState)
{
	state = newState;
}

SettingsScreen& SceneManager::getSettingsScreen()
{
	return settingsScreen;
}

SkillTreeScreen& SceneManager::getSkillTreeScreen()
{
	return skillTreeScreen;
}

MainMenuScreen& SceneManager::getMainMenuScreen()
{
	return mainMenuScreen;
}

bool SceneManager::consumeContinueRequested()
{
	bool value = continueRequested;
	continueRequested = false;
	return value;
}

bool SceneManager::updateMenus(EngineL::InputManager& input, const sf::RenderWindow& window, EngineL::Player& player, WeaponInventory& weaponInventory)
{
	bool startNewRun = false;

	if (state == GameState::MainMenu)
	{
		MainMenuAction action = mainMenuScreen.handleInput(input, window);

		if (action == MainMenuAction::Play)
		{
			state = GameState::Playing;
			startNewRun = true;
		}
		else if (action == MainMenuAction::Continue)
		{
			continueRequested = true;
			state = GameState::Playing;
			startNewRun = true;
		}
		else if (action == MainMenuAction::Settings)
		{
			state = GameState::Settings;
		}
	}
	else if (state == GameState::Settings)
	{
		SettingsAction action = settingsScreen.handleInput(input, window);
		if (action == SettingsAction::Back)
		{
			state = GameState::MainMenu;
		}
	}
	else if (state == GameState::SkillTree)
	{
		SkillTreeAction action = skillTreeScreen.handleInput(input, window, player, weaponInventory);

		if (action == SkillTreeAction::Back)
		{
			state = GameState::Paused;
		}
		else if (action == SkillTreeAction::RestartRun)
		{
			state = GameState::Playing;
			startNewRun = true;
		}
		else if (!skillTreeScreen.isDuringRun() && input.isStartPressed())
		{
			state = GameState::Playing;
			startNewRun = true;
		}
	}
	else if (state == GameState::Paused)
	{
		PauseAction action = pauseScreen.handleInput(input, window);
		if (action == PauseAction::Resume)
		{
			state = GameState::Playing;
		}
		else if (action == PauseAction::SkillTree)
		{
			skillTreeScreen.setDuringRun(true);
			state = GameState::SkillTree;
		}
		else if (action == PauseAction::QuitToMenu)
		{
			state = GameState::MainMenu;
		}
	}
	else if (state == GameState::GameOver)
	{
		GameOverAction action = gameOverScreen.handleInput(input, window);
		if (action == GameOverAction::Retry)
		{
			state = GameState::Playing;
			startNewRun = true;
		}
		if (action == GameOverAction::SkillTree)
		{
			skillTreeScreen.setDuringRun(false);
			state = GameState::SkillTree;
		}
		else if (action == GameOverAction::QuitToMenu)
		{
			state = GameState::MainMenu;
		}
	}

	return startNewRun;
}

void SceneManager::renderCurrentMenu(sf::RenderWindow& window, EngineL::InputManager& input, EngineL::Player& player, float survivedTime)
{
	if (state == GameState::MainMenu)
	{
		mainMenuScreen.render(window);
	}
	else if (state == GameState::Settings)
	{
		settingsScreen.render(window);
	}
	else if (state == GameState::SkillTree)
	{
		skillTreeScreen.render(window, player);
	}
	else if (state == GameState::Paused)
	{
		pauseScreen.render(window);
	}
	else if (state == GameState::GameOver)
	{
		gameOverScreen.render(window, survivedTime);
	}
}