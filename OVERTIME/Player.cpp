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
		s.damage = 0.f;
		s.attackSpeed = 1.f;
		s.critChance = 0.05f;
		s.critDamage = 1.f;
		s.lifesteal = 0.f;
		s.reloadSpeed = 1.f;
		s.regen = 0.f;
		s.difficulty = 1;
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

		if (damageCooldown > 0.f)
		{
			damageCooldown -= deltaTime;

			if (damageCooldown < 0.f)
				damageCooldown = 0.f;
		}
		updateRegen(deltaTime);
	}

	void Player::updateRegen(float deltaTime)
	{
		if (stats.regen <= 0.f)
			return;

		regenTimer += deltaTime;

		if (regenTimer >= 5.f)
		{
			stats.health += static_cast<int>(stats.regen);

			if (stats.health > stats.maxHealth)
				stats.health = stats.maxHealth;

			regenTimer -= 5.f;
		}
	}
	void Player::resetRegenTimer()
	{
		regenTimer = 0;
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
		return inputManager->isMouseButtonPressed(sf::Mouse::Button::Left);
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

	bool Player::hasSecondWeaponSlot() const
	{
		return secondWeaponSlotUnlocked;
	}

	void Player::unlockSecondWeaponSlot()
	{
		secondWeaponSlotUnlocked = true;
	}
	float Player::getMaxTime() const
	{
		return maxTime;
	}

	void Player::increaseMaxTime(float amount)
	{
		maxTime += amount;
	}
}