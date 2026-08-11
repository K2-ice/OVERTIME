#include "pch.h"
#include "Enemy.h"
#include "Renderer.h"
#include <cmath>

namespace EngineL
{
	Enemy::Enemy(float x, float y, Player* player, float difficulty, Map* map)
		: Entity(x, y, 32.f, 32.f, sf::Color::Red)
	{
		this->player = player;
		this->map = map;

		Stats s;
		if (difficulty == 1) {
			s.health = 30.f;
			s.damage = 5.f;
			s.maxHealth = 30.f;
		}
		else {
			s.health = 30.f * 2 * difficulty;
			s.damage = 5.f * 2 * difficulty;
			s.maxHealth = 30.f * 2 * difficulty;
		}
		s.speed = 150.f;
		if (difficulty == 2)
			stats.speed *= 1.25f;
		else if (difficulty == 3)
			stats.speed *= 1.5f;
		else if (difficulty == 4)
			stats.speed *= 2.f;

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

		float moveX = dx * GetStats().speed * speedMultiplier * deltaTime;
		float moveY = dy * GetStats().speed * speedMultiplier * deltaTime;

		tryMove(moveX, moveY);
	}

	bool Enemy::tryMove(float moveX, float moveY) {

		float width = 32.f;
		float height = 32.f;

		bool moved = false;

		sf::Vector2f position = getPosition();
		float newX = position.x + moveX;

		if (map == nullptr || !map->isWallArea(newX, position.y, width, height)) {
			setPosition(newX, position.y);
			moved = true;
		}

		position = getPosition();
		float newY = position.y + moveY;

		if (map == nullptr || !map->isWallArea(position.x, newY, width, height)) {
			setPosition(position.x, newY);
			moved = true;
		}

		return moved;
	}
}