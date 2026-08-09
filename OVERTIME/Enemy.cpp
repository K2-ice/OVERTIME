#include "pch.h"
#include "Enemy.h"
#include "Renderer.h"
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
		s.speed = 150.f;
		s.damage = 7.f;
		setStats(s);
		setTexture("Assets/Characters/Zombie 1/zoimbie1_stand.png");
	}

	void Enemy::update(float deltaTime)
	{
		moveTowardPlayer(deltaTime);

		if (healthBarTimer > 0.f)
		{
			healthBarTimer -= deltaTime;

			if (healthBarTimer < 0.f)
				healthBarTimer = 0.f;
		}
	}

	void Enemy::takeDamage(int amount)
	{
		Entity::takeDamage(amount);
		healthBarTimer = healthBarDuration;
	}

	void Enemy::render(Renderer& renderer)
	{
		if (healthBarTimer > 0.f)
		{
			float barWidth = 40.f;
			float barHeight = 6.f;

			float healthRatio = GetStats().health / GetStats().maxHealth;

			if (healthRatio < 0.f)
				healthRatio = 0.f;

			sf::Vector2f pos = getPosition();

			sf::RectangleShape background;
			background.setSize({ barWidth, barHeight });
			background.setPosition({ pos.x - 4.f, pos.y - 14.f });
			background.setFillColor(sf::Color(60, 60, 60));

			sf::RectangleShape fill;
			fill.setSize({ barWidth * healthRatio, barHeight });
			fill.setPosition({ pos.x - 4.f, pos.y - 14.f });
			fill.setFillColor(sf::Color::Green);

			renderer.drawRectangle(background);
			renderer.drawRectangle(fill);
		}

		Entity::render(renderer);
	}

	bool Enemy::explodesOnContact() const
	{
		return false;
	}

	void Enemy::moveTowardPlayer(float deltaTime, float speedMultiplier)
	{
		float dx = player->getPosition().x - getPosition().x;
		float dy = player->getPosition().y - getPosition().y;

		float length = std::sqrt(dx * dx + dy * dy);

		if (length > 0.f)
		{
			dx /= length;
			dy /= length;
		}

		move(dx * GetStats().speed * speedMultiplier * deltaTime,
			dy * GetStats().speed * speedMultiplier * deltaTime);
	}
}