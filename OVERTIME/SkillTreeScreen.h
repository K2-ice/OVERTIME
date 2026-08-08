#pragma once
#include <SFML/Graphics.hpp>
#include "SkillTree.h"
#include "Player.h"
#include "InputManager.h"

enum class SkillTreeAction
{
	None,
	Back,
	RestartRun
};

class SkillTreeScreen {
public:
	SkillTreeScreen(sf::Font& font);

	void setDuringRun(bool duringRun);
	bool isDuringRun() const;

	SkillTreeAction handleInput(EngineL::InputManager& input, const sf::RenderWindow& window, EngineL::Player& player);
	void render(sf::RenderWindow& window, const EngineL::Player& player);

private:
	bool canAfford(const SkillNode& node, const EngineL::Player& player) const;
	void drawButton(sf::RenderWindow& window, const sf::FloatRect& bounds, const std::string& label);
	void drawConfirmPopup(sf::RenderWindow& window);

	SkillTree skillTree;
	sf::Font& font;
	bool mouseHeld = false;

	bool duringRun = false;
	int pendingNodeIndex = -1;
};