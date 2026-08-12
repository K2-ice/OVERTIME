#include "pch.h"
#include "SettingsScreen.h"

namespace
{
	const sf::FloatRect kVolumeMinusBounds({ 490.f, 280.f }, { 50.f, 50.f });
	const sf::FloatRect kVolumePlusBounds({ 735.f, 280.f }, { 50.f, 50.f });
	const sf::FloatRect kLanguageButtonBounds({ 490.f, 370.f }, { 300.f, 70.f });
	const sf::FloatRect kKeyboardButtonBounds({ 490.f, 460.f }, { 300.f, 70.f });
	const sf::FloatRect kBackButtonBounds({ 490.f, 550.f }, { 300.f, 70.f });
}

SettingsScreen::SettingsScreen(sf::Font& font) : font(font) {
}

int SettingsScreen::getVolume() const {
	return volume;
}

SettingsAction SettingsScreen::handleInput(EngineL::InputManager& input, const sf::RenderWindow& window) {

	bool pressed = input.isMouseButtonPressed(sf::Mouse::Button::Left);
	SettingsAction action = SettingsAction::None;

	if (pressed && !mouseHeld) {
		mouseHeld = true;

		sf::Vector2i mousePixel = input.getMousePosition(window);
		sf::Vector2f mouse(static_cast<float>(mousePixel.x), static_cast<float>(mousePixel.y));

		if (kVolumeMinusBounds.contains(mouse)) {

			volume -= 10;
			if (volume < 0)
				volume = 0;
		}

		else if (kVolumePlusBounds.contains(mouse)) {
			volume += 10;
			if (volume > 100)
				volume = 100;
		}

		else if (kLanguageButtonBounds.contains(mouse)) {
			Language::toggle();
		}

		else if (kKeyboardButtonBounds.contains(mouse)) {
			input.toggleKeyLayout();
		}

		else if (kBackButtonBounds.contains(mouse)) {
			action = SettingsAction::Back;
		}
	}

	if (!pressed)
		mouseHeld = false;

	return action;
}

void SettingsScreen::render(sf::RenderWindow& window, const EngineL::InputManager& input) {

	bool isFrench = Language::current == LanguageOption::French;

	sf::Text title(font);
	title.setCharacterSize(50);
	title.setFillColor(sf::Color::White);
	title.setString(isFrench ? "Parametres" : "Settings");

	sf::FloatRect titleBounds = title.getLocalBounds();
	title.setPosition({
		window.getSize().x / 2.f - titleBounds.size.x / 2.f, 120.f });
	window.draw(title);

	sf::Text volumeLabel(font);
	volumeLabel.setCharacterSize(24);
	volumeLabel.setFillColor(sf::Color::White);
	volumeLabel.setString((isFrench ? "Volume : " : "Volume: ") + std::to_string(volume) + " %");

	sf::FloatRect volumeLabelBounds = volumeLabel.getLocalBounds();
	volumeLabel.setPosition({
		window.getSize().x / 2.f - volumeLabelBounds.size.x / 2.f, 220.f });
	window.draw(volumeLabel);

	drawButton(window, kVolumeMinusBounds, "-");

	drawButton(window, kVolumePlusBounds, "+");

	drawButton(window, kLanguageButtonBounds, isFrench ? "Francais" : "English");

	bool isAzerty = input.getKeyLayout() == EngineL::KeyLayout::AZERTY;
	drawButton(window, kKeyboardButtonBounds, isAzerty ? "AZERTY (ZQSD)" : "QWERTY (WASD)");

	drawButton(window, kBackButtonBounds, isFrench ? "Retour" : "Back");
}

void SettingsScreen::drawButton(sf::RenderWindow& window, const sf::FloatRect& bounds, const std::string& label) {
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