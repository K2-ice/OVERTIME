#include "pch.h"
#include "SettingsScreen.h"

namespace
{
    const sf::FloatRect kBackButtonBounds({ 490.f, 500.f }, { 300.f, 70.f });
}

SettingsScreen::SettingsScreen(sf::Font& font)
    : font(font)
{
}

SettingsAction SettingsScreen::handleInput(EngineL::InputManager& input, const sf::RenderWindow& window)
{
    bool pressed = input.isMouseButtonPressed(sf::Mouse::Button::Left);

    SettingsAction action = SettingsAction::None;

    if (pressed && !mouseHeld)
    {
        mouseHeld = true;

        sf::Vector2i mousePixel = input.getMousePosition(window);
        sf::Vector2f mouse(static_cast<float>(mousePixel.x), static_cast<float>(mousePixel.y));

        if (kBackButtonBounds.contains(mouse))
            action = SettingsAction::Back;
    }

    if (!pressed)
        mouseHeld = false;

    return action;
}

void SettingsScreen::render(sf::RenderWindow& window)
{
    sf::Text title(font);
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::White);
    title.setString("Parametres");

    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition({
        window.getSize().x / 2.f - titleBounds.size.x / 2.f,
        120.f
        });

    window.draw(title);

    sf::Text placeholder(font);
    placeholder.setCharacterSize(22);
    placeholder.setFillColor(sf::Color(180, 180, 180));
    placeholder.setString("Options a venir...");

    sf::FloatRect placeholderBounds = placeholder.getLocalBounds();
    placeholder.setPosition({
        window.getSize().x / 2.f - placeholderBounds.size.x / 2.f,
        250.f
        });

    window.draw(placeholder);

    drawButton(window, kBackButtonBounds, "Retour");
}

void SettingsScreen::drawButton(sf::RenderWindow& window, const sf::FloatRect& bounds, const std::string& label)
{
    sf::RectangleShape box;
    box.setPosition(bounds.position);
    box.setSize(bounds.size);
    box.setFillColor(sf::Color(50, 50, 50));
    box.setOutlineThickness(2.f);
    box.setOutlineColor(sf::Color::White);

    window.draw(box);

    sf::Text text(font);
    text.setCharacterSize(28);
    text.setFillColor(sf::Color::White);
    text.setString(label);

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition({
        bounds.position.x + bounds.size.x / 2.f - textBounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f - textBounds.size.y / 2.f - 5.f
        });

    window.draw(text);
}