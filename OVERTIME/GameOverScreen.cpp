#include "pch.h"
#include "GameOverScreen.h"

namespace
{
	const sf::FloatRect kRetryButtonBounds({ 490.f, 350.f }, { 300.f, 70.f });
	const sf::FloatRect kQuitButtonBounds({ 490.f, 450.f }, { 300.f, 70.f });
}

GameOverScreen::GameOverScreen(sf::Font& font)
	: font(font)
{
}

GameOverAction GameOverScreen::handleInput(EngineL::InputManager& input, const sf::RenderWindow& window)
{
	bool pressed = input.isMouseButtonPressed(sf::Mouse::Button::Left);
	GameOverAction action = GameOverAction::None;

	if (pressed && !mouseHeld)
	{
		mouseHeld = true;

		sf::Vector2i mousePixel = input.getMousePosition(window);
		sf::Vector2f mouse(static_cast<float>(mousePixel.x), static_cast<float>(mousePixel.y));

		if (kRetryButtonBounds.contains(mouse))
			action = GameOverAction::Retry;
		else if (kQuitButtonBounds.contains(mouse))
			action = GameOverAction::QuitToMenu;
	}

	if (!pressed)
		mouseHeld = false;

	return action;
}

void GameOverScreen::render(sf::RenderWindow& window, float survivedTime)
{
	bool isFrench = Language::current == LanguageOption::French;

	sf::Text title(font);
	title.setCharacterSize(60);
	title.setFillColor(sf::Color::Red);
	title.setString(isFrench ? "Partie terminee" : "Game Over");

	sf::FloatRect titleBounds = title.getLocalBounds();
	title.setPosition({
		window.getSize().x / 2.f - titleBounds.size.x / 2.f,
		150.f
		});
	window.draw(title);

	sf::Text timeText(font);
	timeText.setCharacterSize(24);
	timeText.setFillColor(sf::Color::White);

	int seconds = static_cast<int>(survivedTime);
	timeText.setString((isFrench ? "Temps survecu : " : "Time survived: ") + std::to_string(seconds) + " s");

	sf::FloatRect timeBounds = timeText.getLocalBounds();
	timeText.setPosition({
		window.getSize().x / 2.f - timeBounds.size.x / 2.f,
		240.f
		});
	window.draw(timeText);

	drawButton(window, kRetryButtonBounds, isFrench ? "Rejouer" : "Retry");
	drawButton(window, kQuitButtonBounds, isFrench ? "Menu principal" : "Main menu");
}

void GameOverScreen::drawButton(sf::RenderWindow& window, const sf::FloatRect& bounds, const std::string& label)
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