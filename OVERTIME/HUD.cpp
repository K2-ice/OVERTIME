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
	if (name.find("Pompe") != std::string::npos)
		return "Shotgun";
	return name;
}

HUD::HUD(sf::Font& font) : font(font)
{
	hourglassIcon.loadFromFile("Assets/Icons/Board Game Icons/PNG/Default (64px)/hourglass.png");
	heartIcon.loadFromFile("Assets/Icons/Board Game Icons/PNG/Default (64px)/suit_hearts.png");
	starIcon.loadFromFile("Assets/Icons/Game Icons/PNG/White/1x/star.png");

	pistolIcon.loadFromFile("Assets/Weapons/weapon_gun.png");
	smgIcon.loadFromFile("Assets/Weapons/weapon_machine.png");
	shotgunIcon.loadFromFile("Assets/Weapons/weapon_silencer.png");
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

void HUD::drawIcon(sf::RenderWindow& window, const sf::Texture& texture, float x, float y, float size) {
	sf::Vector2u textureSize = texture.getSize();

	if (textureSize.x == 0 || textureSize.y == 0)
		return;

	sf::Sprite icon(texture);

	icon.setScale(sf::Vector2f(
		size / static_cast<float>(textureSize.x),
		size / static_cast<float>(textureSize.y)));

	icon.setPosition({ x, y });

	window.draw(icon);
}

const sf::Texture& HUD::getWeaponIcon(const std::string& weaponName) const {
	if (weaponName == "Mitraillette")
		return smgIcon;

	if (weaponName.find("Pompe") != std::string::npos)
		return shotgunIcon;

	return pistolIcon;
}

void HUD::draw(sf::RenderWindow& window, EngineL::Player& player, EngineL::Weapon* weapon,
	float runTime,
	float maxRunTime,
	bool hasSecondWeaponSlot,
	float bestTime,
	int bestKills,
	int bestSouls) {
	bool isFrench = Language::current == LanguageOption::French;

	sf::Text text(font);

	text.setCharacterSize(30);
	text.setFillColor(sf::Color::White);
	text.setString(formatTime(runTime) + " / " + formatTime(maxRunTime));
	sf::FloatRect bounds = text.getLocalBounds();

	float chronoX = window.getSize().x / 2.f - bounds.size.x / 2.f;
	float chronoY = 20.f;

	drawIcon(window, hourglassIcon, chronoX - 30.f, chronoY - 2.f, 26.f);

	text.setPosition({ chronoX, chronoY });
	window.draw(text);

	drawIcon(window, heartIcon, 10.f, 18.f, 26.f);

	text.setCharacterSize(30);
	text.setFillColor(sf::Color::White);
	text.setString(
		(isFrench ? "PV: " : "HP: ") +
		std::to_string(static_cast<int>(player.GetStats().health)) +
		" / " +
		std::to_string(static_cast<int>(player.GetStats().maxHealth)));

	text.setPosition({ 42.f, 20.f });
	window.draw(text);

	// Arme + munitions
	drawIcon(window, getWeaponIcon(weapon->getName()), 10.f, 62.f, 26.f);

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
	text.setPosition({ 42.f, 62.f });
	window.draw(text);


	std::string recordLabel = isFrench ? "Record : " : "Record: ";
	std::string killsLabel = isFrench ? " kills, " : " kills, ";
	std::string soulsLabel = isFrench ? " souls" : " souls";

	std::string recordText =
		recordLabel +
		formatTime(bestTime) +
		", " +
		std::to_string(bestKills) +
		killsLabel +
		std::to_string(bestSouls) +
		soulsLabel;

	text.setCharacterSize(18);
	text.setFillColor(sf::Color::Yellow);
	text.setString(recordText);

	sf::FloatRect recordBounds = text.getLocalBounds();

	float recordX = window.getSize().x - recordBounds.size.x - 10.f;
	float recordY = 20.f;

	drawIcon(window, starIcon, recordX - 28.f, recordY, 22.f);

	text.setPosition({ recordX, recordY });
	window.draw(text);
}