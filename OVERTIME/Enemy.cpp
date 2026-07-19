#include "pch.h"
#include "Enemy.h"

#include <cmath>

namespace EngineL
{
	Enemy::Enemy(float x, float y, Player* player)
		: Entity(x, y, 32.f, 32.f, sf::Color::Red)
	{
		this->player = player;

		Stats s;
		s.health = 30;
		s.maxHealth = 30;
		s.speed = 120.f;
		s.damage = 5.f;

		setStats(s);
	}

	void Enemy::update(float deltaTime)
	{
		float dx = player->getPosition().x - getPosition().x;
		float dy = player->getPosition().y - getPosition().y;

		float length = std::sqrt(dx * dx + dy * dy);

		if (length > 0.f)
		{
			dx /= length;
			dy /= length;
		}

		move(dx * GetStats().speed * deltaTime,
			dy * GetStats().speed * deltaTime);
	}
}