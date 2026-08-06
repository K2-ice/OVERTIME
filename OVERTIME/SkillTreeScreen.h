#pragma once
#include <SFML/Graphics.hpp>
#include "SkillTree.h"
#include "Player.h"
#include "InputManager.h"

class SkillTreeScreen {

public:

	SkillTreeScreen(sf::Font& font);

	void handleInput(EngineL::InputManager& input, const sf::RenderWindow& window, EngineL::Player& player);
	void render(sf::RenderWindow& window, const EngineL::Player& player);

private:
	SkillTree skillTree;
	sf::Font& font;
	bool mouseHeld = false;
};