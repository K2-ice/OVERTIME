#include "pch.h"
#include "PauseScreen.h"

namespace
{
    const sf::FloatRect kResumeButtonBounds({ 490.f, 260.f }, { 300.f, 70.f });
    const sf::FloatRect kSkillTreeButtonBounds({ 490.f, 350.f }, { 300.f, 70.f });
    const sf::FloatRect kQuitButtonBounds({ 490.f, 440.f }, { 300.f, 70.f });
}

PauseScreen::PauseScreen(sf::Font& font)
    : font(font)
{
}

PauseAction PauseScreen::handleInput(EngineL::InputManager& input, const sf::RenderWindow& window)
{
    if (input.isPausePressed())
        return PauseAction::Resume;

    bool pressed = input.isMouseButtonPressed(sf::Mouse::Button::Left);
    PauseAction action = PauseAction::None;

    if (pressed && !mouseHeld)
    {
        mouseHeld = true;

        sf::Vector2i mousePixel = input.getMousePosition(window);
        sf::Vector2f mouse(static_cast<float>(mousePixel.x), static_cast<float>(mousePixel.y));

        if (kResumeButtonBounds.contains(mouse))
            action = PauseAction::Resume;
        else if (kSkillTreeButtonBounds.contains(mouse))
            action = PauseAction::SkillTree;
        else if (kQuitButtonBounds.contains(mouse))
            action = PauseAction::QuitToMenu;
    }

    if (!pressed)
        mouseHeld = false;

    return action;
}

void PauseScreen::render(sf::RenderWindow& window)
{
    sf::RectangleShape overlay;
    overlay.setSize({ static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y) });
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(overlay);

    sf::Text title(font);
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::White);
    title.setString("Pause");

    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition({
        window.getSize().x / 2.f - titleBounds.size.x / 2.f,
        150.f
        });
    window.draw(title);

    drawButton(window, kResumeButtonBounds, "Reprendre");
    drawButton(window, kSkillTreeButtonBounds, "Arbre de competences");
    drawButton(window, kQuitButtonBounds, "Menu principal");
}

void PauseScreen::drawButton(sf::RenderWindow& window, const sf::FloatRect& bounds, const std::string& label)
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