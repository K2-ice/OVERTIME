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

		shape.setFillColor(sf::Color(255, 215, 0));
	}

	void BelierEnemy::update(float deltaTime)
	{
		if (isCharging)
		{
			// --- Etat 2 : en pleine charge ---
			move(chargeDirX * GetStats().speed * chargeSpeedMultiplier * deltaTime,
				chargeDirY * GetStats().speed * chargeSpeedMultiplier * deltaTime);

			chargeTimer -= deltaTime;

			if (chargeTimer <= 0.f)
			{
				isCharging = false;
			}

			return;
		}

		// --- Etat 1 : poursuite normale ---
		float dx = player->getPosition().x - getPosition().x;
		float dy = player->getPosition().y - getPosition().y;

		float distance = std::sqrt(dx * dx + dy * dy);

		if (distance <= chargeTriggerRange && distance > 0.f)
		{
			// --- Declenchement de la charge ---
			chargeDirX = dx / distance;
			chargeDirY = dy / distance;

			isCharging = true;
			chargeTimer = chargeDuration;
		}
		else
		{
			moveTowardPlayer(deltaTime);
		}
	}
}