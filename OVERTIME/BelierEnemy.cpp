#include "pch.h"
#include "BelierEnemy.h"
#include <cmath>

namespace EngineL
{
	BelierEnemy::BelierEnemy(float x, float y, Player* player)
		: Enemy(x, y, player)
	{
		Stats s;
		s.health = 30;
		s.maxHealth = 30;
		s.speed = 200.f;
		s.damage = 15.f;
		setStats(s);
		setTexture("Assets/Characters/Hitman 1/hitman1_stand.png");

		shape.setFillColor(sf::Color(255, 215, 0));

		retarget();
	}

	void BelierEnemy::retarget()
	{
		float dx = player->getPosition().x - getPosition().x;
		float dy = player->getPosition().y - getPosition().y;

		float distance = std::sqrt(dx * dx + dy * dy);

		if (distance > 0.f)
		{
			chargeDirX = dx / distance;
			chargeDirY = dy / distance;
		}

		chargeTimer = chargeDuration;
	}

	void BelierEnemy::update(float deltaTime)
	{
		move(chargeDirX * GetStats().speed * chargeSpeedMultiplier * deltaTime,
			chargeDirY * GetStats().speed * chargeSpeedMultiplier * deltaTime);

		chargeTimer -= deltaTime;

		if (chargeTimer <= 0.f)
		{
			retarget();
		}
	}
}