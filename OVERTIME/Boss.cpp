#include "pch.h"
#include "Boss.h"
#include <cmath>

namespace EngineL
{
	Boss::Boss(float x, float y, Player* player, float difficulty, Map* map)
		: Enemy(x, y, player, difficulty, map)
	{
		Stats s;
		if (difficulty == 1) {
			s.health = 300.f;
			s.damage = 20.f;
			s.maxHealth = 300.f;
		}
		else {
			s.health = 300.f * 2 * difficulty;
			s.damage = 20.f * 2 * difficulty;
			s.maxHealth = 300.f * 2 * difficulty;
		}
		s.speed = 90.f;
		if (difficulty == 2)
			stats.speed *= 1.25f;
		else if (difficulty == 3)
			stats.speed *= 1.5f;
		else if (difficulty == 4)
			stats.speed *= 2.f;
		setStats(s);
	}

	void Boss::update(float deltaTime)
	{
		moveTowardPlayer(deltaTime, 0.6f);

		if (attackTimer > 0.f)
		{
			attackTimer -= deltaTime;
		}
		else
		{
			readyToFire = true;
			attackTimer = attackCooldown;
		}
	}

	bool Boss::wantsToFire() const
	{
		return readyToFire;
	}

	std::vector<Bullet*> Boss::fire()
	{
		readyToFire = false;

		float dx = player->getPosition().x - getPosition().x;
		float dy = player->getPosition().y - getPosition().y;
		float length = std::sqrt(dx * dx + dy * dy);
		if (length > 0.f)
		{
			dx /= length;
			dy /= length;
		}

		Bullet* bigBullet = new Bullet(
			getPosition().x, getPosition().y,
			dx, dy,
			GetStats().damage,
			sf::Color::Magenta,
			24.f, 24.f);

		return { bigBullet };
	}
}