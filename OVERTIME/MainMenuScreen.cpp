#include "pch.h"
#include "MainMenuScreen.h"

namespace {

	const sf::FloatRect kContinueButtonBounds({ 490.f, 260.f }, { 300.f, 70.f });
	const sf::FloatRect kPlayButtonBounds({ 490.f, 350.f }, { 300.f, 70.f });
	const sf::FloatRect kSettingsButtonBounds({ 490.f, 440.f }, { 300.f, 70.f });
	const sf::FloatRect kQuitButtonBounds({ 490.f, 530.f }, { 300.f, 70.f });
}

MainMenuScreen::MainMenuScreen(sf::Font& font) : font(font) {

	continueIcon.loadFromFile("Assets/Icons/Game Icons Expansion/PNG/White/1x/diamond.png");
	playIcon.loadFromFile("Assets/Icons/Game Icons Expansion/PNG/White/1x/flag.png");
	settingsIcon.loadFromFile("Assets/Icons/Game Icons/PNG/White/1x/gear.png");
	quitIcon.loadFromFile("Assets/Icons/Game Icons/PNG/White/1x/exitLeft.png");
}

void MainMenuScreen::setSavedSouls(int souls) {
	savedSouls = souls;
}

MainMenuAction MainMenuScreen::handleInput(EngineL::InputManager& input, const sf::RenderWindow& window) {
	bool pressed = input.isMouseButtonPressed(sf::Mouse::Button::Left);
	MainMenuAction action = MainMenuAction::None;

	if (pressed && !mouseHeld) {
		mouseHeld = true;

		sf::Vector2i mousePixel = input.getMousePosition(window);
		sf::Vector2f mouse(static_cast<float>(mousePixel.x), static_cast<float>(mousePixel.y));

		if (savedSouls > 0 && kContinueButtonBounds.contains(mouse))
			action = MainMenuAction::Continue;
		else if (kPlayButtonBounds.contains(mouse))
			action = MainMenuAction::Play;
		else if (kSettingsButtonBounds.contains(mouse))
			action = MainMenuAction::Settings;
		else if (kQuitButtonBounds.contains(mouse))
			action = MainMenuAction::Quit;
	}

	if (!pressed)
		mouseHeld = false;

	return action;
}

void MainMenuScreen::render(sf::RenderWindow& window) {
	bool isFrench = Language::current == LanguageOption::French;

	sf::Text title(font);
	title.setCharacterSize(60);
	title.setFillColor(sf::Color::Magenta);
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

		drawButton(window, kContinueButtonBounds, label, continueIcon);
	}

	drawButton(window, kPlayButtonBounds, isFrench ? "Nouvelle partie" : "New game", playIcon);
	drawButton(window, kSettingsButtonBounds, isFrench ? "Parametres" : "Settings", settingsIcon);
	drawButton(window, kQuitButtonBounds, isFrench ? "Quitter" : "Quit", quitIcon);
}

void MainMenuScreen::drawButton(sf::RenderWindow& window, const sf::FloatRect& bounds, const std::string& label, const sf::Texture& icon) {
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

	float iconSize = 28.f;
	float spacing = 10.f;

	float contentWidth = iconSize + spacing + textBounds.size.x;

	float startX = bounds.position.x + bounds.size.x / 2.f - contentWidth / 2.f;
	float centerY = bounds.position.y + bounds.size.y / 2.f;

	sf::Vector2u textureSize = icon.getSize();

	if (textureSize.x > 0 && textureSize.y > 0) {
		sf::Sprite iconSprite(icon);

		iconSprite.setScale(sf::Vector2f(
			iconSize / static_cast<float>(textureSize.x),
			iconSize / static_cast<float>(textureSize.y)));

		iconSprite.setPosition({ startX, centerY - iconSize / 2.f });

		window.draw(iconSprite);
	}

	text.setPosition({ startX + iconSize + spacing,	centerY - textBounds.size.y / 2.f - 5.f });
	window.draw(text);
}