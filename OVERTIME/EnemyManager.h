#pragma once

#include <vector>
#include "Enemy.h"

namespace EngineL
{
	class EnemyManager
	{
	public:
		std::vector<Enemy*>& getEnemies();

	private:
		std::vector<Enemy*> enemies;
	};
}