#include "pch.h"
#include "Player.h"

#include <cmath>

namespace EngineL
{
	Player::Player(float x, float y, InputManager* inputManager)
		: Entity(x, y, 32.f, 32.f, sf::Color::Blue)
	{
		this->inputManager = inputManager;
		Stats s;
		s.health = 100;
		s.maxHealth = 100;
		s.speed = 300.f;
		s.damage = 10.f;
		s.attackSpeed = 1.f;

		setStats(s);
	}

	void Player::update(float deltaTime)
	{
		float dx = 0.f;
		float dy = 0.f;

		if (inputManager->isMovingUp())
			dy -= 1.f;

		if (inputManager->isMovingDown())
			dy += 1.f;

		if (inputManager->isMovingLeft())
			dx -= 1.f;

		if (inputManager->isMovingRight())
			dx += 1.f;

		float length = std::sqrt(dx * dx + dy * dy);

		if (length > 0.f)
		{
			dx /= length;
			dy /= length;
		}

		move(dx * GetStats().speed * deltaTime,
			dy * GetStats().speed * deltaTime);

		if (shootCooldown > 0.f)
		{
			shootCooldown -= deltaTime;

			if (shootCooldown < 0.f)
				shootCooldown = 0.f;
		}
	}

	void Player::aimAt(float mouseX, float mouseY)
	{
		float playerX = getPosition().x;
		float playerY = getPosition().y;

		float dirX = mouseX - playerX;
		float dirY = mouseY - playerY;

		float length = std::sqrt(dirX * dirX + dirY * dirY);

		if (length > 0.f)
		{
			shootDirectionX = dirX / length;
			shootDirectionY = dirY / length;
		}
	}

	bool Player::wantsToShoot() const
	{
		return inputManager->isMouseButtonPressed(sf::Mouse::Button::Left)
			&& shootCooldown <= 0.f;
	}

	void Player::resetShootCooldown()
	{
		shootCooldown = GetStats().attackSpeed;
	}

	bool Player::canTakeDamage() const
	{
		return damageCooldown <= 0.f;
	}

	void Player::resetDamageCooldown()
	{
		damageCooldown = damageCooldownMax;
	}


	float Player::getShootDirectionX() const
	{
		return shootDirectionX;
	}

	float Player::getShootDirectionY() const
	{
		return shootDirectionY;
	}
	int Player::getSouls() const
	{
		return souls;
	}

	void Player::addSouls(int amount)
	{
		souls += amount;
	}

	bool Player::spendSouls(int amount)
	{
		if (souls < amount)
			return false;

		souls -= amount;
		return true;
	}
}