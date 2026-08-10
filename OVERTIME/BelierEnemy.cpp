#include "pch.h"
#include "BelierEnemy.h"
#include <cmath>

namespace EngineL
{
	BelierEnemy::BelierEnemy(float x, float y, Player* player, float difficulty, Map* map)
		: Enemy(x, y, player, difficulty, map)
	{
		Stats s;
		if (difficulty == 1) {
			s.health = 30.f;
			s.damage = 15.f;
			s.maxHealth = 30.f;
		}
		else {
			s.health = 30.f * 2 * difficulty;
			s.damage = 15.f * 2 * difficulty;
			s.maxHealth = 30.f * 2 * difficulty;
		}
		s.speed = 200.f;
		if (difficulty == 2)
			stats.speed *= 1.25f;
		else if (difficulty == 3)
			stats.speed *= 1.5f;
		else if (difficulty == 4)
			stats.speed *= 2.f;
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
		float moveX = chargeDirX * GetStats().speed * chargeSpeedMultiplier * deltaTime;
		float moveY = chargeDirY * GetStats().speed * chargeSpeedMultiplier * deltaTime;

		tryMove(moveX, moveY);

		chargeTimer -= deltaTime;

		if (chargeTimer <= 0.f)
		{
			retarget();
		}
	}
}