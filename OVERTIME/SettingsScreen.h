#pragma once
#include <SFML/Graphics.hpp>
#include "InputManager.h"
#include "Language.h"

enum class SettingsAction {

	None,
	Back
};
class SettingsScreen {

public:

	SettingsScreen(sf::Font& font);
	SettingsAction handleInput(EngineL::InputManager& input, const sf::RenderWindow& window);

	void render(sf::RenderWindow& window);
	int getVolume() const;

private:

	void drawButton(sf::RenderWindow& window, const sf::FloatRect& bounds, const std::string& label);

	sf::Font& font;

	bool mouseHeld = false;
	bool draggingSlider = false;

	int volume = 100;
};