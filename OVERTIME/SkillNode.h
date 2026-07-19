#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

enum class SkillType
{
	Damage,
	Health,
	Speed,
	AttackSpeed,
	Range,

	UnlockBow,
	UnlockStaff,
	UnlockGun
};

struct SkillNode
{
	SkillType type;

	std::string name;
	std::string description;

	float amount;

	int level = 0;
	int maxLevel = 10;

	int cost = 10;

	float priceMultiplier = 1.25f;

	bool unlocked = false;

	sf::Vector2f position;

	std::vector<int> children;
};