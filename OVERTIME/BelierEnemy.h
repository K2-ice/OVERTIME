#pragma once
#include "Enemy.h"

namespace EngineL
{
	class BelierEnemy : public Enemy
	{
	public:
		BelierEnemy(float x, float y, Player* player);

		void update(float deltaTime) override;

	private:
		bool isCharging = false;
		float chargeDirX = 0.f;
		float chargeDirY = 0.f;

		float chargeTimer = 0.f;
		float chargeDuration = 2.f;

		float chargeTriggerRange = 250.f;
		float chargeSpeedMultiplier = 2.5f;
	};
}