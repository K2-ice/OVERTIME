#pragma once
#include "Enemy.h"
#include "Bullet.h"
#include <vector>

namespace EngineL {

	class Boss : public Enemy
	{
	public:
		Boss(float x, float y, Player* player, float difficulty, Map* map);

		void update(float deltaTime) override;

		bool wantsToFire() const;
		std::vector<Bullet*> fire();

	private:
		float attackTimer = 0.f;
		float attackCooldown = 3.f;
		bool readyToFire = false;
	};
}