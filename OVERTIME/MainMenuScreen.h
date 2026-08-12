#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "InputManager.h"
#include "Language.h"

enum class MainMenuAction {

	None,
	Play,
	Continue,
	Settings,
	Quit
};

class MainMenuScreen {

public:

	MainMenuScreen(sf::Font& font);

	void setSavedSouls(int souls);

	MainMenuAction handleInput(EngineL::InputManager& input, const sf::RenderWindow& window);
	void render(sf::RenderWindow& window);

private:

	void drawButton(sf::RenderWindow& window, const sf::FloatRect& bounds, const std::string& label, const sf::Texture& icon);

	sf::Font& font;
	bool mouseHeld = false;

	int savedSouls = 0;

	sf::Texture continueIcon;
	sf::Texture playIcon;
	sf::Texture settingsIcon;
	sf::Texture quitIcon;
};