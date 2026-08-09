#include "pch.h"
#include "KamikazeEnemy.h"

namespace EngineL
{
	KamikazeEnemy::KamikazeEnemy(float x, float y, Player* player)
		: Enemy(x, y, player)
	{
		Stats s;
		s.health = 15;
		s.maxHealth = 15;
		s.speed = 260.f;
		s.damage = 20.f;
		setStats(s);
		setTexture("Assets/Characters/Robot 1/robot1_stand.png");

		shape.setFillColor(sf::Color(255, 140, 0));
	}

	bool KamikazeEnemy::explodesOnContact() const
	{
		return true;
	}
}