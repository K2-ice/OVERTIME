#pragma once
#include <SFML/Graphics.hpp>
#include "SkillTree.h"
#include "Player.h"
#include "InputManager.h"
#include <algorithm>

class SkillTreeScreen {

public:

	SkillTreeScreen(sf::Font& font);

	void handleInput(EngineL::InputManager& input, const sf::RenderWindow& window, EngineL::Player& player, WeaponInventory& weaponinventory);
	void render(sf::RenderWindow& window, const EngineL::Player& player);

private:
	SkillTree skillTree;
	sf::Font& font;
	bool mouseHeld = false;
	sf::View treeView;

	float zoom = 1.f;
	const float minZoom = 0.5f;
	const float maxZoom = 1.5f;

	bool dragging = false;
	bool leftPressed = false;
	sf::Vector2i dragStart;
	sf::Vector2i lastMousePos;
};