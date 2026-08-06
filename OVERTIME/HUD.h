#pragma once

#include <string>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Player.h"
#include "Weapon.h"

class HUD {

public:

    HUD(sf::Font& font);

    void draw(sf::RenderWindow& window, EngineL::Player& player, EngineL::Weapon* weapon,

        float runTime,
        float maxRunTime,
        bool hasSecondWeaponSlot);

private:

    std::string formatTime(float seconds) const;

    sf::Font& font;
};