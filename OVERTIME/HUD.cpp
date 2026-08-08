#include "pch.h"
#include "HUD.h"
#include <sstream>
#include <iomanip>

HUD::HUD(sf::Font& font)
    : font(font)
{
}

std::string HUD::formatTime(float seconds) const
{
    int totalSeconds = static_cast<int>(seconds);
    int minutes = totalSeconds / 60;
    int secs = totalSeconds % 60;
    std::ostringstream stream;
    stream << std::setfill('0')
        << std::setw(2) << minutes
        << ":"
        << std::setw(2) << secs;
    return stream.str();
}

void HUD::drawHealthBar(sf::RenderWindow& window, EngineL::Player& player)
{
    float health = player.GetStats().health;
    float maxHealth = player.GetStats().maxHealth;

    float barWidth = 250.f;
    float barHeight = 28.f;
    float barX = 10.f;
    float barY = 20.f;

    float filledWidth = (health / maxHealth) * barWidth;

    if (filledWidth < 0.f)
        filledWidth = 0.f;

    sf::RectangleShape background;
    background.setPosition({ barX, barY });
    background.setSize({ barWidth, barHeight });
    background.setFillColor(sf::Color(60, 60, 60));
    background.setOutlineThickness(2.f);
    background.setOutlineColor(sf::Color::White);
    window.draw(background);

    sf::RectangleShape fill;
    fill.setPosition({ barX, barY });
    fill.setSize({ filledWidth, barHeight });
    fill.setFillColor(sf::Color::Green);
    window.draw(fill);

    sf::Text text(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);
    text.setString(
        std::to_string(static_cast<int>(health)) +
        " / " +
        std::to_string(static_cast<int>(maxHealth)));
    text.setPosition({ barX + 8.f, barY + 4.f });
    window.draw(text);
}

void HUD::draw(
    sf::RenderWindow& window,
    EngineL::Player& player,
    EngineL::Weapon* weapon,
    float runTime,
    float maxRunTime,
    bool hasSecondWeaponSlot)
{
    sf::Text text(font);

    // Chrono
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::White);
    text.setString(formatTime(runTime) + " / " + formatTime(maxRunTime));
    sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition({
        window.getSize().x / 2.f - bounds.size.x / 2.f,
        20.f
        });
    window.draw(text);

    // Vie
    drawHealthBar(window, player);

    // Arme + munitions
    text.setCharacterSize(20);
    text.setFillColor(weapon->isReloading() ? sf::Color(255, 165, 0) : sf::Color::White);

    std::string ammoText;

    if (weapon->isReloading())
    {
        ammoText = "Rechargement...";
    }
    else
    {
        ammoText = std::to_string(weapon->getCurrentAmmo()) + " / ";
        ammoText += weapon->hasInfiniteReserve() ? "..." : std::to_string(weapon->getReserveAmmo());
    }

    std::string switchHint = hasSecondWeaponSlot ? "  [1/2, R]" : "  [R]";

    text.setString("Arme : " + weapon->getName() + "  (" + ammoText + ")" + switchHint);
    text.setPosition({ 10.f, 60.f });
    window.draw(text);
}