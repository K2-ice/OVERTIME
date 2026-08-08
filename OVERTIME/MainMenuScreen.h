#pragma once
#include <SFML/Graphics.hpp>
#include "InputManager.h"

enum class MainMenuAction
{
    None,
    Play,
    Settings
};

class MainMenuScreen
{
public:
    MainMenuScreen(sf::Font& font);

    MainMenuAction handleInput(EngineL::InputManager& input, const sf::RenderWindow& window);
    void render(sf::RenderWindow& window);

private:
    void drawButton(sf::RenderWindow& window, const sf::FloatRect& bounds, const std::string& label);

    sf::Font& font;
    bool mouseHeld = false;
};