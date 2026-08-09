#include "pch.h"
#include "Boss.h"
#include <cmath>

namespace EngineL
{
	Boss::Boss(float x, float y, Player* player)
		: Enemy(x, y, player)
	{
		Stats s;
		s.health = 300;
		s.maxHealth = 300;
		s.speed = 90.f;
		s.damage = 20.f;
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