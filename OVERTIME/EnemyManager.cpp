#include "pch.h"
#include "EnemyManager.h"

namespace EngineL
{
	std::vector<Enemy*>& EnemyManager::getEnemies()
	{
		return enemies;
	}
}