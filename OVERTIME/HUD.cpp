#include "pch.h"
#include "HUD.h"

#include <sstream>
#include <iomanip>

std::string translateWeaponName(const std::string& name, bool isFrench) {

	if (isFrench)
		return name;

	if (name == "Pistolet")
		return "Gun";

	if (name == "Mitraillette")
		return "SMG";

	if (name == "Fusil à Pompe")
		return "Shotgun";

	return name;
}

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

void HUD::draw(sf::RenderWindow& window, EngineL::Player& player, EngineL::Weapon* weapon, float runTime, float maxRunTime, bool hasSecondWeaponSlot)
{
	bool isFrench = Language::current == LanguageOption::French;

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


	text.setCharacterSize(30);
	text.setFillColor(sf::Color::White);

	text.setString(
		(isFrench ? "PV: " : "HP: ") +
		std::to_string(static_cast<int>(player.GetStats().health)) +
		" / " +
		std::to_string(static_cast<int>(player.GetStats().maxHealth)));

	text.setPosition({ 10.f, 20.f });

	window.draw(text);


	text.setCharacterSize(20);
	text.setFillColor(weapon->isReloading() ? sf::Color(255, 165, 0) : sf::Color::White);

	std::string ammoText;

	if (weapon->isReloading())
	{
		ammoText = isFrench ? "Rechargement..." : "Reloading...";
	}
	else
	{
		ammoText = std::to_string(weapon->getCurrentAmmo()) + " / " + std::to_string(weapon->getMagazineSize());

		if (isFrench)
			ammoText += weapon->hasInfiniteReserve() ? " (reserve infinie)" : " (reserve : " + std::to_string(weapon->getReserveAmmo()) + ")";
		else
			ammoText += weapon->hasInfiniteReserve() ? " (infinite reserve)" : " (reserve: " + std::to_string(weapon->getReserveAmmo()) + ")";
	}

	std::string switchHint = hasSecondWeaponSlot ? "  [1/2, R]" : "  [R]";

	text.setString((isFrench ? "Arme : " : "Weapon: ") + translateWeaponName(weapon->getName(), isFrench) + "  (" + ammoText + ")" + switchHint);

	text.setPosition({ 10.f, 60.f });

	window.draw(text);
}

