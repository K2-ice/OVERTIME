// GameOverScreen.h
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "InputManager.h"

enum class GameOverAction
{
    None,
    Retry,
    QuitToMenu
};

class GameOverScreen
{
public:
    GameOverScreen(sf::Font& font);

    GameOverAction handleInput(EngineL::InputManager& input, const sf::RenderWindow& window);
    void render(sf::RenderWindow& window, float survivedTime);

private:
    void drawButton(sf::RenderWindow& window, const sf::FloatRect& bounds, const std::string& label);

    sf::Font& font;
    bool mouseHeld = false;
};