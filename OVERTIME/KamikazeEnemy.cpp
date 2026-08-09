#include "pch.h"
#include "KamikazeEnemy.h"

namespace EngineL
{
	KamikazeEnemy::KamikazeEnemy(float x, float y, Player* player, float difficulty, Map* map)
		: Enemy(x, y, player, difficulty, map)
	{
		Stats s;
		if (difficulty == 1) {
			s.health = 15.f;
			s.damage = 10.f;
			s.maxHealth = 60.f;
		}
		else {
			s.health = 15.f * 2 * difficulty;
			s.damage = 10.f * 2 * difficulty;
			s.maxHealth = 15.f * 2 * difficulty;
		}
		s.speed = 260.f;
		if (difficulty == 2)
			stats.speed *= 1.25f;
		else if (difficulty == 3)
			stats.speed *= 1.5f;
		else if (difficulty == 4)
			stats.speed *= 2.f;
		setStats(s);
		setTexture("Assets/Characters/Robot 1/robot1_stand.png");

		shape.setFillColor(sf::Color(255, 140, 0));
	}

	bool KamikazeEnemy::explodesOnContact() const
	{
		return true;
	}
}