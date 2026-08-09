#pragma once
#include <SFML/Graphics.hpp>
#include "SkillTree.h"
#include "Player.h"
#include "InputManager.h"
#include "Language.h"
#include <algorithm>

enum class SkillTreeAction {

	None,
	Back,
	RestartRun
};

class SkillTreeScreen {

public:
	SkillTreeScreen(sf::Font& font);

	void setDuringRun(bool duringRun);
	bool isDuringRun() const;

	SkillTreeAction handleInput(
		EngineL::InputManager& input,
		const sf::RenderWindow& window,
		EngineL::Player& player,
		WeaponInventory& weaponInventory
	);

	void render(sf::RenderWindow& window, const EngineL::Player& player);

private:

	bool canAfford(const SkillNode& node, const EngineL::Player& player) const;
	void drawButton(sf::RenderWindow& window, const sf::FloatRect& bounds, const std::string& label);
	void drawConfirmPopup(sf::RenderWindow& window);

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

	bool duringRun = false;
	int pendingNodeIndex = -1;
};