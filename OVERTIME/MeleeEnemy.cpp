#include "pch.h"
#include "MeleeEnemy.h"

namespace EngineL
{
	MeleeEnemy::MeleeEnemy(float x, float y, Player* player)
		: Enemy(x, y, player)
	{
		Stats s;
		s.health = 60;
		s.maxHealth = 60;
		s.speed = 110.f;
		s.damage = 12.f;
		setStats(s);
		setTexture("Assets/Characters/Zombie 2/zombie2_stand.png");

		shape.setFillColor(sf::Color(150, 0, 150));
	}
}