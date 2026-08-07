#include "pch.h"
#include "WeaponPickup.h"

namespace EngineL
{
	WeaponPickup::WeaponPickup(float x, float y, const std::string& weaponId)
		: Entity(x, y, 20.f, 20.f, sf::Color::Magenta)
	{
		this->weaponId = weaponId;
	}

	void WeaponPickup::update(float deltaTime)
	{
	}

	const std::string& WeaponPickup::getWeaponId() const
	{
		return weaponId;
	}
}