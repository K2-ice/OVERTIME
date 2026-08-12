#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Player.h"
#include "Weapon.h"
#include "Language.h"
#include "Objective.h"

class HUD {
public:
	HUD(sf::Font& font);

	void draw(
		sf::RenderWindow& window,
		EngineL::Player& player,
		EngineL::Weapon* weapon,
		float runTime,
		float maxRunTime,
		bool hasSecondWeaponSlot,
		float bestTime,
		int bestKills,
		int bestSouls);

	void drawObjectives(sf::RenderWindow& window, const std::vector<Objective>& objectives);

private:
	std::string formatTime(float seconds) const;
	void drawIcon(sf::RenderWindow& window, const sf::Texture& texture, float x, float y, float size);
	const sf::Texture& getWeaponIcon(const std::string& weaponName) const;

	sf::Font& font;

	sf::Texture hourglassIcon;
	sf::Texture heartIcon;
	sf::Texture starIcon;

	sf::Texture pistolIcon;
	sf::Texture smgIcon;
	sf::Texture shotgunIcon;
};