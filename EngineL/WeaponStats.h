#pragma once
#include <string>

struct WeaponStats {

	std::string name;

	float fireRate = 1.f;
	float damage = 10.f;
	int pelletCount = 1;
	float spreadAngleDegrees = 0.f;
	int magazineSize = 10;
	float reloadTime = 1.f;
	int reserveAmmo = -1;
};