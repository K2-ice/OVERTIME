#include "pch.h"
#include "Weapon.h"
#include <cmath>
#include <algorithm>

namespace EngineL
{
	Weapon::Weapon(const WeaponStats& stats) {

		name = stats.name;
		fireRate = stats.fireRate;
		damage = stats.damage;
		pelletCount = stats.pelletCount;
		spreadAngleDegrees = stats.spreadAngleDegrees;
		magazineSize = stats.magazineSize;
		reloadTime = stats.reloadTime;
		currentAmmo = magazineSize;

		reserveAmmo = stats.reserveAmmo;
		maxReserveAmmo = stats.reserveAmmo;
		infiniteReserve = (reserveAmmo < 0);
	}

	void Weapon::update(float deltaTime)	{

		if (cooldown > 0.f) {
			cooldown -= deltaTime;

			if (cooldown < 0.f)
				cooldown = 0.f;
		}

		if (reloading) {

			reloadTimer -= deltaTime;

			if (reloadTimer <= 0.f) {
				reloading = false;

				if (infiniteReserve) {
					currentAmmo = magazineSize;
				}

				else {
					int needed = magazineSize - currentAmmo;
					int amountToLoad = std::min(needed, reserveAmmo);

					currentAmmo += amountToLoad;
					reserveAmmo -= amountToLoad;
				}
			}
		}
	}

	bool Weapon::canFire() const {
		return cooldown <= 0.f && !reloading && currentAmmo > 0;
	}

	std::vector<Bullet*> Weapon::fire(float x, float y, float dirX, float dirY, float damageBonus, float fireRateBonus, Map* map) {

		std::vector<Bullet*> bullets;

		if (!canFire())
			return bullets;

		cooldown = 1.f / (fireRate * fireRateBonus);
		currentAmmo--;

		if (pelletCount <= 1) {
			bullets.push_back(new Bullet(x, y, dirX, dirY, damage + damageBonus, map));
		}

		else {

			float baseAngle = std::atan2(dirY, dirX);
			float spreadRadians = spreadAngleDegrees * (3.14159265f / 180.f);

			float startAngle = baseAngle - spreadRadians * 0.5f;
			float angleStep = spreadRadians / (pelletCount - 1);

			for (int i = 0; i < pelletCount; i++) {

				float angle = startAngle + angleStep * i;
				float pelletDirX = std::cos(angle);
				float pelletDirY = std::sin(angle);

				bullets.push_back(new Bullet(x, y, pelletDirX, pelletDirY, damage + damageBonus, map));
			}
		}

		return bullets;
	}

	void Weapon::startReload(float reloadspeed) {

		if (reloading)
			return;

		if (currentAmmo >= magazineSize)
			return;

		if (!infiniteReserve && reserveAmmo <= 0)
			return;

		reloading = true;
		reloadTimer = reloadTime * reloadspeed;
	}

	void Weapon::resetAmmo() {
		currentAmmo = magazineSize;
		if (!infiniteReserve)
			reserveAmmo = maxReserveAmmo;
		reloading = false;
		reloadTimer = 0.f;
		cooldown = 0.f;
	};

	bool Weapon::isReloading() const {
		return reloading;
	}

	void Weapon::increaseCapacity(float magazineIncrease, float reserveIncrease) {
		magazineSize += static_cast<int>(magazineIncrease);
		maxReserveAmmo += static_cast<int>(reserveIncrease);

		currentAmmo = magazineSize;

		if (!infiniteReserve)
			reserveAmmo = maxReserveAmmo;
	}

	std::string Weapon::getName() const {
		return name;
	}

	int Weapon::getCurrentAmmo() const {
		return currentAmmo;
	}

	int Weapon::getMagazineSize() const {
		return magazineSize;
	}

	int Weapon::getReserveAmmo() const {
		return reserveAmmo;
	}

	bool Weapon::hasInfiniteReserve() const {
		return infiniteReserve;
	}
}