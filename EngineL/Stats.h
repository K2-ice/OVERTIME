#pragma once

struct Stats {

	float speed = 500.f;

	float damage = 10.f;

	float health = 100.f;
	float maxHealth = 100.f;

	float attackSpeed = 1.f;
	float critChance = 0.f;
	float critDamage = 1.f;
	float lifesteal = 0.f;
	float reloadSpeed = 1.f;
	float regen = 0.f;

	int difficulty = 1;
	int maxDifficulty = 1;
	bool hasShotgun = false;
	bool hasSubmachinegun = false;
};