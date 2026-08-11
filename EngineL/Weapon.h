#pragma once
#include <vector>
#include <string>
#include "Bullet.h"
#include "WeaponStats.h"

namespace EngineL
{
	class Weapon {

	public:

		Weapon(const WeaponStats& stats);

		void update(float deltaTime);
		bool canFire() const;

		std::vector<Bullet*> fire(float x, float y, float dirX, float dirY, float damageBonus, float fireRateBonus, Map* map = nullptr);

		void startReload(float reloadspeed);
		void resetAmmo();
		bool isReloading() const;

		std::string getName() const;
		int getCurrentAmmo() const;
		int getMagazineSize() const;
		int getReserveAmmo() const;
		bool hasInfiniteReserve() const;
		void increaseCapacity(float magazineIncrease, float reserveIncrease);

	private:

		std::string name;
		float fireRate;
		float damage;
		int pelletCount;
		float spreadAngleDegrees;

		int magazineSize;
		int currentAmmo;
		float reloadTime;
		float reloadTimer = 0.f;
		bool reloading = false;

		int reserveAmmo;
		int maxReserveAmmo;
		bool infiniteReserve;

		float cooldown = 0.f;
	};
}