#pragma once
#include "Entity.h"
#include "InputManager.h"
#include "Map.h"
namespace EngineL
{
	class Player : public Entity
	{
	public:

		Player(float x, float y, InputManager* inputManager, Map* map);

		void update(float deltaTime) override;
		void aimAt(float mouseX, float mouseY);

		bool wantsToShoot() const;
		float getShootDirectionX() const;
		float getShootDirectionY() const;

		int getSouls() const;

		void addSouls(int amount);

		bool spendSouls(int amount);
		bool canTakeDamage() const;

		void resetDamageCooldown();

		bool hasSecondWeaponSlot() const;

		void unlockSecondWeaponSlot();

		float getMaxTime() const;
		void increaseMaxTime(float amount);

		void updateRegen(float deltaTime);
		void resetRegenTimer();

	private:

		InputManager* inputManager;
		Map* map;

		float shootDirectionX = 0.f;
		float shootDirectionY = 0.f;

		int souls = 0;

		float damageCooldown = 0.f;
		float damageCooldownMax = 0.5f;

		bool secondWeaponSlotUnlocked = false;
		float maxTime = 10.f;

		float regenTimer = 0.f;
	};
}