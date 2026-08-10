#include "pch.h"
#include "MeleeEnemy.h"

namespace EngineL
{
	MeleeEnemy::MeleeEnemy(float x, float y, Player* player, float difficulty, Map* map)
		: Enemy(x, y, player, difficulty, map)
	{
		Stats s;
		if (difficulty == 1) {
			s.health = 60.f;
			s.damage = 12.f;
			s.maxHealth = 60.f;
		}
		else {
			s.health = 60.f * 2 * difficulty;
			s.damage = 12.f * 2 * difficulty;
			s.maxHealth = 60.f * 2 * difficulty;
		}
		s.speed = 110.f;
		if (difficulty == 2)
			stats.speed *= 1.25f;
		else if (difficulty == 3)
			stats.speed *= 1.5f;
		else if (difficulty == 4)
			stats.speed *= 2.f;
		setStats(s);
		setTexture("Assets/Characters/Zombie 2/zombie2_stand.png");

		shape.setFillColor(sf::Color(150, 0, 150));
	}
}