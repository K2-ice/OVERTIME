#pragma once
#include <SFML/Graphics.hpp>
#include "InputManager.h"
#include "Language.h"

enum class PauseAction
{
	None,
	Resume,
	SkillTree,
	QuitToMenu
};

class PauseScreen
{
public:
	PauseScreen(sf::Font& font);

	PauseAction handleInput(EngineL::InputManager& input, const sf::RenderWindow& window);
	void render(sf::RenderWindow& window);

private:
	void drawButton(sf::RenderWindow& window, const sf::FloatRect& bounds, const std::string& label);

	sf::Font& font;
	bool mouseHeld = false;
};