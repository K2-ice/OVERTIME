#include "pch.h"
#include "MainMenuScreen.h"

namespace
{
    const sf::FloatRect kContinueButtonBounds({ 490.f, 260.f }, { 300.f, 70.f });
    const sf::FloatRect kPlayButtonBounds({ 490.f, 350.f }, { 300.f, 70.f });
    const sf::FloatRect kSettingsButtonBounds({ 490.f, 440.f }, { 300.f, 70.f });
}

MainMenuScreen::MainMenuScreen(sf::Font& font)
    : font(font)
{
}

void MainMenuScreen::setSavedSouls(int souls)
{
    savedSouls = souls;
}

MainMenuAction MainMenuScreen::handleInput(EngineL::InputManager& input, const sf::RenderWindow& window)
{
    bool pressed = input.isMouseButtonPressed(sf::Mouse::Button::Left);
    MainMenuAction action = MainMenuAction::None;

    if (pressed && !mouseHeld)
    {
        mouseHeld = true;

        sf::Vector2i mousePixel = input.getMousePosition(window);
        sf::Vector2f mouse(static_cast<float>(mousePixel.x), static_cast<float>(mousePixel.y));

        if (savedSouls > 0 && kContinueButtonBounds.contains(mouse))
            action = MainMenuAction::Continue;
        else if (kPlayButtonBounds.contains(mouse))
            action = MainMenuAction::Play;
        else if (kSettingsButtonBounds.contains(mouse))
            action = MainMenuAction::Settings;
    }

    if (!pressed)
        mouseHeld = false;

    return action;
}

void MainMenuScreen::render(sf::RenderWindow& window)
{
    bool isFrench = Language::current == LanguageOption::French;

    sf::Text title(font);
    title.setCharacterSize(60);
    title.setFillColor(sf::Color::White);
    title.setString("OVERTIME");

    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition({
        window.getSize().x / 2.f - titleBounds.size.x / 2.f,
        120.f
        });
    window.draw(title);

    if (savedSouls > 0)
    {
        std::string label = isFrench
            ? "Continuer (" + std::to_string(savedSouls) + " Ames)"
            : "Continue (" + std::to_string(savedSouls) + " Souls)";

        drawButton(window, kContinueButtonBounds, label);
    }

    drawButton(window, kPlayButtonBounds, isFrench ? "Nouvelle partie" : "New game");
    drawButton(window, kSettingsButtonBounds, isFrench ? "Parametres" : "Settings");
}

void MainMenuScreen::drawButton(sf::RenderWindow& window, const sf::FloatRect& bounds, const std::string& label)
{
    sf::RectangleShape box;
    box.setPosition(bounds.position);
    box.setSize(bounds.size);
    box.setFillColor(sf::Color(50, 50, 50));
    box.setOutlineThickness(2.f);
    box.setOutlineColor(sf::Color::White);
    window.draw(box);

    sf::Text text(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setString(label);

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition({
        bounds.position.x + bounds.size.x / 2.f - textBounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f - textBounds.size.y / 2.f - 5.f
        });
    window.draw(text);
}